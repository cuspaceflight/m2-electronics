/*
 * L3G4200D Driver 
 * Cambridge University Spaceflight
 *
 * Assumption made that the address 
 * of the I2C driver is &I2CD2.
 *
 * In config, must call wakeup function
 * on DRDY/INT2 interrupt.
 */

#include <stdlib.h>
#include "microsd.h"
#include "config.h"
#include "state_estimation.h"
#include "l3g4200d.h"  

/*
 * The 7 bit address of the sensor is 110100x.
 * x is determined by whether the SDO pin is
 * connected to ground or the supply.
 * Here the assumption is made that the connection
 * is to ground.
 */
#define L3G4200D_I2C_WRITE_ADDR    0xD0 
#define L3G4200D_I2C_READ_ADDR     0xD1

static Thread *tpL3G4200D = NULL;
/* static float counts_to_units ; - needs to be determined */

/* Transmit data to sensor */
static bool_t l3g4200d_transmit(uint8_t *buf)
{
    size_t n = 2;  /* Transmit 2 bytes */
    systime_t timeout;
    msg_t rv;

    /* Determine timeout in systicks (ms) - UNSURE ABOUT THIS */
    timeout = n / 100 + 10; 

    /* Transmit message */
    rv = i2cMasterTransmitTimeout(&I2CD2, L3G4200D_I2C_WRITE_ADDR, buf, 
	                              n, NULL, 0, timeout);
								  
    if (rv == RDY_OK)
        return TRUE;
    else
        return FALSE;

}

/* 
 * Read data about angular rate from sensor into buffer.
 * Do so through a transmit operation.
 * Place output data(24 bytes) into a (different) buffer.
 * Enable auto-increment by setting MSB of register address to 1.
 */

static bool_t l3g4200d_receive(uint8_t *buf, int8_t *buf_data)
{
    msg_t rv;
    buf[0] = 0xA8; /* Hex address of first register to be read from */
    buf[1] = NULL;
	
	/* Timeout is somewhat arbitrary- CHECK THIS */
    size_t n = 24;  /* Receive 24 bytes */
    systime_t timeout;
	
	/* Determine timeout in systicks (ms) */
    timeout = n / 100 + 20; 
	
	
    rv = i2cMasterTransmitTimeout(&I2CD2, L3G4200D_I2C_READ_ADDR, buf, 
	                          1, buf_data, 24, timeout);
    
    if  (rv == RDY_OK)
	      return TRUE;
  	else
	      return FALSE;
}

static bool_t l3g4200d_init(uint8_t *buf)
{
    bool_t success;
  
    /* Configure CTRL_REG1: Highest Data Rate- 800 Hz, Bandwidth- 50 (UNSURE) */
    buf[0] = 0x20;
    buf[1] = 0xE7;
    success = l3g4200d_transmit(buf) ; 
								  
    /* 
     * Configure CTRL_REG3:
     * This allows interrupts on DRDY/INT2
     * when the FIFO has filled to a watermark
     * level specified in the FIFO_CTRL_REG.
     */

    buf[0] = 0x22;
    buf[1] = 0x04;
    success &= l3g4200d_transmit(buf) ;
	
	/* Configure CTRL_REG5: Enable FIFO operation */
    buf[0] = 0x24;
    buf[1] = 0x40;
    success &= l3g4200d_transmit(buf) ; 
	
    /* 
     * Configure FIFO_CTRL_REG: 
     * Set the FIFO to operate in Stream Mode.
     * Set the Watermark level to be 4.
     * Interrupts on DRDY/INT2 occur when
     * the FIFO has filled to this level.
     */
	
    buf[0] = 0x2E;	
    buf[1] = 0x44;
    success &= l3g4200d_transmit(buf) ; 
	
    return success;
  
}

/* 
 * buf_data contains 12 pairs of 8 bit entries. (4 * 3 pairs)
 * A set of 3 pairs contains data about the angular rate for the X,Y, and Z axes.
 * Function converts the 4 X,Y, and Z values to three float representing
 * the rotation rate about each axis.
 * Multiplication by constant necessary to account for the 
 * conversion factor from counts measured into the correct unit.
 */
static void l3g4200d_rotation_convert(int8_t *buf_data, float *rotation)
{
    for (i =0; i<6 i+=2)
    {
        float temp = 0;
        int16_t accumulator = 0;
        int j =0;
		
	    for(j=0; j<24; j+=6)
		{
	        int16_t total_rotation;
	      	total_rotation = (int16_t(buf_data[i+1+j]) << 8) 
		                 | (uint16_t(buf_data[i+j])) ;
	      	accumulator += total_rotation;
 	    }
		
        temp = 0.25 * float(accumulator) * counts_to_units ;
 	    rotation[i] = temp;
    }
}

/* 
 * Interrupt handler- wake up when DRDY deasserted 
 * Relevant pin needs defining in main/config
 */
void l3g4200d_wakeup(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;
    chSysLockFromIsr();
    if(tpL3G4200D != NULL) 
    {
        chSchReadyI(tpL3G4200D);
        tpL3G4200D = NULL;
    }
    chSysUnlockFromIsr();
}

msg_t l3g4200d_thread(void *arg)
{
    (void)arg;
    const int bufsize_1 = 2;
    const int bufsize_2 = 24;
    
    uint8_t buf[bufsize_1];
    int8_t buf_data[bufsize_2];
    float rotation[3] ;
	
    chRegSetThreadName("L3G4200D");
	
    /* 
     * Reset Gyroscope so it's in a known state 
     * Uncomment afer pin allocation completed
     *
     *
     * 
     * palClearPad(GPIOB, GPIOB_GPS_RESET); 
     * chThdSleepMilliseconds(100);
     * palSetPad(GPIOB, GPIOB_GPS_RESET); 
     * chThdSleepMilliseconds(500);
     */

    i2cStart(&I2CD2, &i2cconfig); /* left as I2CD2 for now. */

    if(!l3g4200d_init(buf) 
    {
        while(1) chThdSleepMilliseconds(5);
    }
    
	i2cStart(&I2CD2, &i2cconfig); /* left as I2CD2 for now. */
	
	while(TRUE)
	{   
		if (l3g4200d_receive(buf, buf_data))
		{
		    l3g4200d_rotation_convert(buf_data, rotation);
		    microsd_log_s16(CHAN_IMU_GYRO /*define*/, 
			            rotation[0], rotation[1], rotation[2], 0);
		    /*define this state estimation function */
                    state_estimation_new_gyro(rotation[0], 
			                       rotation[1], rotation[2]);
		}
		
		else
		    chThdSleepMilliseconds(20);
		
		
		/* Sleep until DRDY */
		chSysLock();
		tpL3G4200D = chThdSelf();
		chSchGoSleepTimeoutS(THD_STATE_SUSPENDED, 100);
		chSysUnlock();
	}
	
    return (msg_t)NULL;
}

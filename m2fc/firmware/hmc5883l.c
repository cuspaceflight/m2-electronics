/*
 * HMC5883L Driver 
 * Cambridge University Spaceflight
 */

#include <stdlib.h>

#include "hal.h"
#include "datalogging.h"
#include "hmc5883l.h"
#include "tweeter.h"


#define HMC5883L_I2C_ADDR       0x1E

#define HMC5883L_RA_OUT         0x03
#define HMC5883L_RA_CONFIG_A    0x00
#define HMC5883L_RA_CONFIG_B    0x01
#define HMC5883L_RA_MODE        0x02
#define HMC5883L_RA_ID_A        0x0A
#define HMC5883L_RA_ID_B        0x0B
#define HMC5883L_RA_ID_C        0x0C

static Thread *tpHMC5883L = NULL;

int16_t global_magno[3];

static const I2CConfig i2cconfig = {
	OPMODE_I2C, 100000, STD_DUTY_CYCLE
};


/* Transmit data to sensor (used in init function only) */
static bool_t hmc5883l_transmit(uint8_t address, uint8_t data) {
    uint8_t buffer[2];
    buffer[0] = address;
    buffer[1] = data;

    /* Transmit message */
    return (i2cMasterTransmitTimeout(&I2CD2, HMC5883L_I2C_ADDR, buffer, 2, NULL, 0, 1000) == RDY_OK);
}

/* When called, will read 6 bytes of XZY data into buf_data: [XH][XL][ZH][ZL][YH][YL] NOTE THE ORDER! */
static bool_t hmc5883l_receive(uint8_t *buf_data) {
    uint8_t address = HMC5883L_RA_OUT;
    return (i2cMasterTransmitTimeout(&I2CD2, HMC5883L_I2C_ADDR, &address, 1, buf_data, 6, 1000) == RDY_OK);
}

static bool_t hmc5883l_init(void) {
    bool_t success = TRUE;
    
    /* Config Reg A: Data Rates
       Send 00010000 [0][00 = mov avg ovr 1 samp][100 = 15Hz][00 = normal operation] */
    success &= hmc5883l_transmit(HMC5883L_RA_CONFIG_A,0x10);

    /* Config Reg B: Gain
       Send 00100000 [001 = 1090 (reciprocal) gain][00000] */
    success &= hmc5883l_transmit(HMC5883L_RA_CONFIG_B,0x20);

    /* Mode Reg: Operating Mode
       Send 00000000 [000000][00 = cont mode] */
    success &= hmc5883l_transmit(HMC5883L_RA_MODE,0x00);

    return success;
}

/* Checks the ID of the Magno to ensure we're actually talking to the Magno and not some other component. */
static bool_t hmc58831_ID_check(void) {
    uint8_t buf;
    uint8_t id_reg;
    bool_t success = TRUE;
    
    /* Forcefully try to read each ID register since it seems like they don't exist. */
    id_reg = HMC5883L_RA_ID_A;
    if(i2cMasterTransmitTimeout(&I2CD2, HMC5883L_I2C_ADDR, &id_reg, 1, &buf, 1, 1000) == RDY_OK) {
        success &= (buf == 0x48);
    } else {
        return FALSE;
    }
    
    id_reg = HMC5883L_RA_ID_B;
    if(i2cMasterTransmitTimeout(&I2CD2, HMC5883L_I2C_ADDR, &id_reg, 1, &buf, 1, 1000) == RDY_OK) {
        success &= (buf == 0x34);
    } else {
        return FALSE;
    }
    
    id_reg = HMC5883L_RA_ID_C;
    if(i2cMasterTransmitTimeout(&I2CD2, HMC5883L_I2C_ADDR, &id_reg, 1, &buf, 1, 1000) == RDY_OK) {
        success &= (buf == 0x33);
    } else {
        return FALSE;
    }
    return success;
}


/* Conversion to meaningful units. Output in miligauss. (1G = 1e-4T)*/
static void hmc5883l_field_convert(uint8_t *buf_data, float *field) {
    /* 0.73 mG/LSB for 1370 (reciprocal) gain
       0.92 mG/LSB for 1090 (reciprocal) gain
       1.22 mG/LSB for 820 (reciprocal) gain
       1.52 mG/LSB for 660 (reciprocal) gain
       2.27 mG/LSB for 440 (reciprocal) gain
       2.56 mG/LSB for 390 (reciprocal) gain
       3.03 mG/LSB for 330 (reciprocal) gain
       4.35 mG/LSB for 230 (reciprocal) gain */
    static float sensitivity = 0.92f;
    int16_t concatenated_field;
    
    int i;
    for (i=0; i<3; i++)
    {
        concatenated_field = (buf_data[(i*2)] << 8) | (buf_data[(i*2+1)]);
        global_magno[i] = concatenated_field;
    	field [i] = ((float)concatenated_field) * sensitivity;
    }
    /* Note the order of received is X,Z,Y, so re-arrangement is done here. */
    float temp;
    temp = global_magno[1];
    global_magno[1] = global_magno[2];
    global_magno[2] = temp;
}

/* 
 * Interrupt handler- wake up when DRDY deasserted 
 * Relevant pin needs defining in main/config
 */
void hmc5883l_wakeup(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;
    chSysLockFromIsr();
    if(tpHMC5883L != NULL && tpHMC5883L->p_state != THD_STATE_READY) 
    {
        chSchReadyI(tpHMC5883L);
    }
    tpHMC5883L = NULL;
    chSysUnlockFromIsr();
}

msg_t hmc5883l_thread(void *arg)
{
    (void)arg;

    uint8_t buf_data[6];
    float field[3];
	
    chRegSetThreadName("HMC5883L");

    i2cStart(&I2CD2, &i2cconfig);
    
    while (!hmc58831_ID_check()) {
        tweeter_set_error(ERROR_MAGNO, true);
        chThdSleepMilliseconds(500);
    }
    tweeter_set_error(ERROR_MAGNO, false);
    
    /* Initialise the settings. */
    while (!hmc5883l_init()) {
        tweeter_set_error(ERROR_MAGNO, true);
        chThdSleepMilliseconds(500);
    }
    tweeter_set_error(ERROR_MAGNO, false);
    
    while (TRUE)
    {
        /* Sleep until DRDY */
        chSysLock();
        tpHMC5883L = chThdSelf();
        chSchGoSleepTimeoutS(THD_STATE_SUSPENDED, 100);
        tpHMC5883L = NULL;
        chSysUnlock();
        
        /* Pull data from magno into buf_data. */
        if (hmc5883l_receive(buf_data)) {
            tweeter_set_error(ERROR_MAGNO, false);
            hmc5883l_field_convert(buf_data, field);
            log_s16(CHAN_IMU_MAGNO, field[0], field[1], field[2], 0); 
            /*define this state estimation function 
            state_estimation_new_magno(field[0], 
			       field[1], field[2]); */
        } else {
            chThdSleepMilliseconds(20);
            tweeter_set_error(ERROR_MAGNO, true);
        }
    }
}

/*
 * uBlox GPS receiver
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

/*
 * TODO
 *
 * * Read large buffers up to the number of bytes available from the ublox
 * * Run each new byte through a decoding state machine
 * * SM calls various functions to deal with each type of received message
 * * * Either ACK/NAK, PVT, or perhaps also CFG-NAV5
 * * Should poll the device's NAV5 at least once to ensure flight mode
 * * PVT updates get sent to central dispatch
 * * How should setting config find out about an ACK/NAK?
 */

#include "ublox.h"

/* uBlox I2C addresses */
#define UBLOX_I2C_ADDR        0x42
#define UBLOX_I2C_BYTES_AVAIL 0xFD
#define UBLOX_I2C_DATA        0xFF

/* UBX sync bytes */
#define UBX_SYNC1 0xB5
#define UBX_SYNC2 0x62

/* UBX Classes */
#define UBX_NAV 0x01
#define UBX_RXM 0x02
#define UBX_INF 0x04
#define UBX_ACK 0x05
#define UBX_CFG 0x06
#define UBX_UPD 0x09
#define UBX_MON 0x0A
#define UBX_AID 0x0B
#define UBX_TIM 0x0D
#define UBX_MGA 0x13
#define UBX_LOG 0x21
#define NMEA_CLASS 0xF0

/* Selection of UBX IDs */
#define UBX_CFG_MSG  0x01
#define UBX_CFG_NAV5 0x24
#define UBX_NAV_PVT  0x07
#define NMEA_GGA 0x00
#define NMEA_GLL 0x01
#define NMEA_GSA 0x02
#define NMEA_GSV 0x03
#define NMEA_RMC 0x04
#define NMEA_VTG 0x05

/* UBX Decoding State Machine States */
typedef enum {
    STATE_IDLE = 0, STATE_SYNC1, STATE_SYNC2,
    STATE_CLASS, STATE_ID, STATE_L1, STATE_L2,
    STATE_PAYLOAD, STATE_PAYLOAD_DONE, STATE_CK_A,
    NUM_STATES
} ubx_state;

/* Structs for various UBX messages */

/* UBX-CFG-NAV5
 * Set navigation fix settings.
 * Notably includes changing dynamic mode to "Airborne 4G".
 */
typedef struct {
    union {
        uint8_t data;
        uint8_t sync1;
    };
    uint8_t sync2, class, id;
    uint16_t length;
    union {
        uint8_t payload[36];
        struct {
            uint16_t mask;
            uint8_t dyn_model;
            uint8_t fix_mode;
            int32_t fixed_alt;
            uint32_t fixed_alt_var;
            int8_t min_elev;
            uint8_t dr_limit;
            uint16_t p_dop, t_dop, p_acc, t_acc;
            uint8_t static_hold_thres;
            uint8_t dgps_timeout;
            uint8_t cno_thresh_num_svs, cno_thresh;
            uint16_t reserved;
            uint16_t static_hold_max_dist;
            uint8_t utc_standard;
            uint8_t reserved3;
            uint32_t reserved4;
        };
    };
    uint8_t ck_a, ck_b;
} ubx_cfg_nav5_t;

/* UBX-CFG-MSG
 * Change rate (or disable) automatic delivery of messages
 * to the current port.
 */
typedef struct {
    union {
        uint8_t data;
        uint8_t sync1;
    };
    uint8_t sync2, class, id;
    uint16_t length;
    union {
        uint8_t payload[3];
        struct {
            uint8_t msg_class;
            uint8_t msg_id;
            uint8_t rate;
        };
    };
    uint8_t ck_a, ck_b;
} ubx_cfg_msg_t;

/* UBX-ACK
 * ACK/NAK messages after trying to set a config.
 */
typedef struct {
    union {
        uint8_t data;
        uint8_t sync1;
    };
    uint8_t sync2, class, id;
    uint16_t length;
    union {
        uint8_t payload[2];
        struct {
            uint8_t cls_id;
            uint8_t msg_id;
        };
    };
    uint8_t ck_a, ck_b;
} ubx_ack_t;

/* UBX-NAV-PVT
 * Contains fix quality, position and time information.
 * Everything you want in one message.
 */
typedef struct {
    union {
        uint8_t data;
        uint8_t sync1;
    };
    uint8_t sync2, class, id;
    uint16_t length;
    union {
        uint8_t payload[92];
        struct {
            uint32_t i_tow;
            uint16_t year;
            uint8_t month, day, hour, minute, second;
            uint8_t valid;
            uint32_t t_acc;
            int32_t nano;
            uint8_t fix_type;
            uint8_t flags;
            uint8_t reserved1;
            uint8_t num_sv;
            int32_t lon, lat;
            int32_t height, h_msl;
            uint32_t h_acc, v_acc;
            int32_t velN, velE, velD, gspeed;
            int32_t head_mot;
            uint32_t s_acc;
            uint32_t head_acc;
            uint16_t p_dop;
            uint16_t reserved2;
            uint32_t reserved3;
            int32_t head_veh;
            uint32_t reserved4;
        };
    };
    uint8_t ck_a, ck_b;
} ubx_nav_pvt_t;

/* Computes the Fletcher-8 checksum over buf, using its length fields
 * to determine how much to read and where to insert the new checksum.
 */
static void ublox_checksum(uint8_t *buf)
{
    uint16_t plen, i;
    uint8_t ck_a, ck_b;

    /* Check SYNC bytes are correct */
    if(buf[0] != UBX_SYNC1 && buf[1] != UBX_SYNC2)
        return;

    /* Extract payload length */
    plen = *((uint16_t*)buf)[2];

    /* Run Fletcher-8 algorithm */
    for(i=2; i<plen+6; i++) {
        ck_a += buf[i];
        ck_b += ck_a;
    }

    /* Write new checksum to the buffer */
    buf[plen+6] = ck_a;
    buf[plen+7] = ck_b;
}

/* Transmit a UBX message over the I2C port.
 * Message length is determined from the UBX length field.
 * Checksum is added automatically.
 */
static bool_t ublox_transmit(uint8_t *buf)
{
    size_t n;
    systime_t timeout;
    msg_t rv;

    /* Add checksum to outgoing message */
    ublox_checksum(buf);

    /* Determine length and thus suitable timeout in systicks (ms) */
    n = 8 + *((uint16_t*)buf)[2];
    timeout = n / 100 + 10;

    /* Transmit message */
    rv = i2cMasterTransmitTimeout(&I2CD1, UBLOX_I2C_ADDR, data, n,
                                  NULL, 0, timeout);
    return rv == RDY_OK;
}

/* As ublox_transmit, but checks for an ACK afterwards, returns false if NAK.
 */
static bool_t ublox_transmit_cfg(uint8_t *buf)
{
    bool_t rv = ublox_transmit(buf);
    /* TODO: Look for ACK/NAK message */
    /* Can't just look at latest message. We'll have to keep discarding
     * messages from the buffer until we find one that we want. And then hope
     * it's not NMEA or some rubbish.
     * Perhaps clear the buffer _before_ transmitting, then transmit and maybe
     * that will help.
     */
    return rv; 
}

/* Attempt to read one or more messages from the uBlox.
 * Returns 0 if no messages were read, else returns the number of bytes
 * read. Will read at most bufsize bytes into buf.
 */
static size_t ublox_receive(uint8_t *buf, size_t bufsize)
{
    uint16_t bytes_available;
    uint8_t bytes_available_addr = UBLOX_I2C_BYTES_AVAIL;
    systime_t timeout;
    msg_t rv;

    /* Set the read address to 0xFD, the start of "bytes available",
     * then read those two bytes. Timeout of 2ms should be plenty.
     */
    rv = i2cMasterTransmitTimeout(&I2CD1, UBLOX_I2C_ADDR,
                                  &bytes_available_addr, 1, buf, 2, 2);
    if(rv != RDY_OK)
        return 0;

    bytes_available = ((uint16_t)buf)[0];
    
    /* If there's data to read, try and read it.
     * After the last two reads, the read register will already be 0xFF.
     */
    if(bytes_available > 0) {
        /* Don't overflow the read buffer. We'll just read some more later. */
        if(bytes_available > bufsize)
            bytes_available = bufsize;
        timeout = bytes_available / 100 + 10;
        rv = i2cMasterReceiveTimeout(&I2CD1, UBLOX_I2C_ADDR,
                                     buf, bytes_available, timeout);
        if(rv == RDY_OK)
            return bytes_available;
        else
            return 0;
    }

    /* No data to read. */
    return 0;
}

/* Run the first `num_new_bytes` bytes in `buf` through the UBX decoding state
 * machine. Note that this function preserves static state and dispatches new
 * messages as appropriate once received.
 */
static void ubox_state_machine(uint8_t *buf, size_t num_new_bytes)
{
    size_t i;
    static ubx_state state = STATE_IDLE;

    static uint8_t class, id;
    static uint16_t length;
    static uint16_t length_remaining;
    static char payload[128];
    static uint8_t ck_a, ck_b;
    
    for(i = 0; i < num_new_bytes; i++) {
        uint8_t b = buf[i];

        switch(state) {
            case STATE_IDLE:
                if(b == UBX_SYNC1)
                    state = STATE_SYNC1;
                break;

            case STATE_SYNC1:
                if(b == UBX_SYNC2)
                    state = STATE_SYNC2;
                else
                    state = STATE_IDLE;
                break;

            case STATE_SYNC2:
                class = b;
                state = STATE_CLASS;
                break;

            case STATE_CLASS:
                id = b;
                state = STATE_ID;
                break;

            case STATE_ID:
                length = (uint16_t)b << 8;
                state = STATE_L1;
                break;

            case STATE_L1:
                length |= (uint16_t)b;
                length_remaining = length;
                state = STATE_L2;
                break;

            case STATE_PAYLOAD:
                if(length_remaining)
                    payload[length - length_remaining--] = b;
                else
                    state = STATE_PAYLOAD_DONE;
                break;
            
            case STATE_PAYLOAD_DONE:
                ck_a = b;
                state = STATE_CK_A;
                break;

            case STATE_CK_A:
                cb_b = b;
                state = STATE_IDLE;
                switch(class) {
                    case UBX_ACK:
                        if(id == 0x00) {
                            /* NAK */
                        } else if(id == 0x01) {
                            /* ACK */
                        } else {
                            /* TODO SAD */
                        }
                        break;
                    case UBX_NAV:
                        if(id == 0x07) {
                            /* PVT */
                        } else {
                            /* TODO SAD */
                        }
                        break;
                    case UBX_CFG:
                        if(id == 0x24) {
                            /* NAV5 */
                        } else {
                            /* TODO SAD */
                        }
                        break;
                    default:
                        /* TODO sad */
                        break;
                }
                break;

            default:
                state = STATE_IDLE;
                /* TODO sad */
                break;

        }
    }
}

static bool_t ublox_init(uint8_t *buf, size_t bufsize)
{
    ubx_cfg_nav5_t nav5;
    ubx_cfg_msg_t msg;
    bool_t success = TRUE;

    /* Set to Airborne <4g dynamic mode */
    nav5.sync1 = UBX_SYNC1;
    nav5.sync2 = UBX_SYNC2;
    nav5.class = UBX_CFG;
    nav5.id = UBX_CFG_NAV5;
    msg.length = 36;

    nav5.mask = (1<<0);
    nav5.dyn_model = 8;
    nav5.reserved  = 0;
    nav5.reserved3 = 0;
    nav5.reserved4 = 0;

    success &= ublox_transmit_cfg(&nav5.data);

    /* Disable NMEA messages to I2C */
    msg.sync1 = UBX_SYNC1;
    msg.sync2 = UBX_SYNC2;
    msg.class = UBX_CFG;
    msg.id = UBX_CFG_MSG;
    msg.length = 3;

    msg.msg_class = NMEA_CLASS;
    msg.msg_id    = NMEA_GGA;
    msg.rate      = 0;
    success &= ublox_transmit_cfg(&msg.data);

    msg.msg_class = NMEA_CLASS;
    msg.msg_id    = NMEA_GLL;
    msg.rate      = 0;
    success &= ublox_transmit_cfg(&msg.data);

    msg.msg_class = NMEA_CLASS;
    msg.msg_id    = NMEA_GSA;
    msg.rate      = 0;
    success &= ublox_transmit_cfg(&msg.data);

    msg.msg_class = NMEA_CLASS;
    msg.msg_id    = NMEA_GSV;
    msg.rate      = 0;
    success &= ublox_transmit_cfg(&msg.data);

    msg.msg_class = NMEA_CLASS;
    msg.msg_id    = NMEA_RMC;
    msg.rate      = 0;
    success &= ublox_transmit_cfg(&msg.data);

    msg.msg_class = NMEA_CLASS;
    msg.msg_id    = NMEA_VTG;
    msg.rate      = 0;
    success &= ublox_transmit_cfg(&msg.data);

    msg.msg_class = UBX_NAV;
    msg.msg_id    = UBX_NAV_PVT;
    msg.rate      = 1;
    success &= ublox_transmit_cfg(&msg.data);

    /* Clear the current I2C buffer */
    while(ublox_try_receive_raw(buf, bufsize));
}

/* TODO: Validate timings against AN4235 */
/* Magic I2C timing numbers. Computed via reference manual. */
static const I2CConfig i2cconfig = {
    STM32_TIMINGR_PRESC(8)   |
    STM32_TIMINGR_SCLDEL(12) | STM32_TIMINGR_SDADEL(15) |
    STM32_TIMINGR_SCLH(79)   | STM32_TIMINGR_SCLL(79),
    0, 0
};

msg_t ublox_thread(void* arg)
{
    (void)arg;
    const int bufsize = 256;
    uint8_t rawbuf[bufsize];
    uint8_t msgbuf[bufsize];

    i2cStart(&I2CD1, &i2cconfig);

    if(!ublox_init(rawbuf, msgbuf, bufsize)) {
        /* TODO: be sad */
    }

    while(TRUE) {
        if(!ublox_try_receive(rawbuf, msgbuf, bufsize)) {
            chThdSleepMilliseconds(50);
        }
    }
}

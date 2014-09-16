/*
 * uBlox GPS receiver
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
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

/* Structs for various UBX messages */
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
    systime_t to;
    msg_t rv;

    /* Add checksum to outgoing message */
    ublox_checksum(buf);

    /* Determine length and thus suitable timeout in systicks (ms) */
    n = 8 + *((uint16_t*)buf)[2];
    to = n / 100 + 10;

    /* Transmit message */
    rv = i2cMasterTransmitTimeout(&I2CD1, UBLOX_I2C_ADDR, data, n,
                                  NULL, 0, to);
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
 * Returns FALSE if no messages were read, else returns the number of bytes
 * read.
 */
static size_t ublox_try_receive_raw(uint8_t *buf, size_t bufsize)
{
    uint16_t bytes_available;
    uint8_t bytes_available_addr = UBLOX_I2C_BYTES_AVAIL;
    systime_t to;
    msg_t rv;

    /* Set the read address to 0xFD, the start of "bytes available",
     * then read those two bytes. Timeout of 2ms should be plenty.
     */
    rv = i2cMasterTransmitTimeout(&I2CD1, UBLOX_I2C_ADDR,
                                  &bytes_available_addr, 1, buf, 2, 2);
    if(rv != RDY_OK)
        return FALSE;

    bytes_available = ((uint16_t)buf)[0];
    
    /* If there's data to read, try and read it.
     * After the last two reads, the read register will already be 0xFF.
     */
    if(bytes_available > 0) {
        /* Don't overflow our read buffer. We'll just read some more later. */
        if(bytes_available > bufsize)
            bytes_available = bufsize;
        to = bytes_available / 100 + 10;
        rv = i2cMasterReceiveTimeout(&I2CD1, UBLOX_I2C_ADDR,
                                     buf, bytes_available, to);
        return rv == RDY_OK;
    }

    /* No data to read. */
    return FALSE;
}

/* TODO
 * Read messages from the uBlox.
 * Ideally we would read at most one message at a time, return the type of
 * message, and ensure that buf pointed to the start. Might need two buffers -
 * one for the single-message and one longer-lived one that we copy i2c data
 * into.
 */
static void ublox_try_read(uint8_t *buf, size_t bufsize)
{
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

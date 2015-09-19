/*
 * M2 Telemetry
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#ifndef M2_TELEM_H
#define M2_TELEM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Packet Structure ===========================================================
 */
typedef struct {
    union {
        char        c[8];
        int64_t     i64;
        uint64_t    u64;
        int32_t     i32[2];
        uint32_t    u32[2];
        int16_t     i16[4];
        uint16_t    u16[4];
        int8_t      i8[8];
        uint8_t     u8[8];
        float       f[2];
        double      d;
    };
    uint32_t timestamp;
    uint8_t metadata;
    uint8_t channel;
    uint16_t checksum;
} __attribute__((packed)) TelemPacket;

/* Checksums ==================================================================
 *
 * CRC-16-CCITT checksum algorithm. Polynomial 0x1021, initial value 0x0000,
 * no final XOR or other mangling.
 */
void m2telem_write_checksum(TelemPacket *packet);
bool m2telem_check_checksum(TelemPacket *packet);

/* Framing ====================================================================
 *
 * Frame messages by prefixing a 0x7E, then escaping any occurance of 0x7E or
 * 0x7D with a 0x7D and XOR with 0x20. Deframe by inverting this operation.
 */

/*
 * Enframe a packet into a buffer which must be able to hold 33 bytes.
 * Or, if you're feeling lucky (odds are good that not every single byte will
 * require escaping...) you could use a 32 byte buffer.
 *
 * Sets `buf_len` to the number of bytes in the frame.
 */
void m2telem_enframe(TelemPacket* pkt, uint8_t* buf, size_t* buf_len);

/*
 * Deframe a packet from a buffer that may contain fragments of packets.
 *
 * Set `buf_len` to the number of new bytes in the buffer. Feed with at most 32
 * bytes per call, so that no more than one packet will have to be returned.
 *
 * `state` is a DeframeState where `s` is initialised to 0 and then persisted
 * between calls to `m2telem_deframe` with respect to the same input buffer.
 *
 * Returns true when a complete packet has been written into `out`, or
 * false if not.
 */
typedef struct {
    uint8_t buf[32];
    uint8_t idx;
    uint8_t s;
} DeframeState;
bool m2telem_deframe(uint8_t* buf, size_t buf_len,
                     DeframeState* state, TelemPacket* pkt);

/*
 * Channel constants ==========================================================
 *
 * The _GROUP constants allow checking which group a channel is in by comparing
 * against the channel masked with M2T_CH_GROUP_MASK.
 */

#define M2T_CH_GROUP_MASK           (0xF0)

#define M2T_CH_GROUP_SYS            (0x00)
#define M2T_CH_SYS_INIT             (0x00)
#define M2T_CH_SYS_VERSION          (0x01)

#define M2T_CH_GROUP_CAL            (0x10)
#define M2T_CH_CAL_TFREQ            (0x10)
#define M2T_CH_CAL_LG_ACCEL         (0x11)
#define M2T_CH_CAL_HG_ACCEL         (0x12)
#define M2T_CH_CAL_BARO_1           (0x13)
#define M2T_CH_CAL_BARO_2           (0x14)

#define M2T_CH_GROUP_IMU            (0x20)
#define M2T_CH_IMU_LG_ACCEL         (0x20)
#define M2T_CH_IMU_HG_ACCEL         (0x21)
#define M2T_CH_IMU_BARO             (0x22)
#define M2T_CH_IMU_GYRO             (0x23)
#define M2T_CH_IMU_MAGNO            (0x24)

#define M2T_CH_GROUP_ADC            (0x30)
#define M2T_CH_ADC_BATT             (0x30)
#define M2T_CH_ADC_STRAIN           (0x31)
#define M2T_CH_ADC_THERMO           (0x32)

#define M2T_CH_GROUP_STATE          (0x40)
#define M2T_CH_STATE_MISSION        (0x40)

#define M2T_CH_GROUP_SE             (0x50)
#define M2T_CH_SE_T_H               (0x50)
#define M2T_CH_SE_V_A               (0x51)
#define M2T_CH_SE_PRESSURE          (0x52)
#define M2T_CH_SE_ACCEL             (0x53)

#define M2T_CH_GROUP_PYRO           (0x60)
#define M2T_CH_PYRO_CONT            (0x60)
#define M2T_CH_PYRO_FIRE            (0x61)

#define M2T_CH_GROUP_GPS            (0x70)
#define M2T_CH_GPS_TIME             (0x71)
#define M2T_CH_GPS_POS              (0x72)
#define M2T_CH_GPS_ALT              (0x73)
#define M2T_CH_GPS_STATUS           (0x74)

#endif /* M2_TELEM_H */

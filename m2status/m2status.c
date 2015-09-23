#include "m2status.h"
#include <stdbool.h>
#include "hal.h"

SystemStatus M2FCBodyStatus = {0};
SystemStatus M2FCNoseStatus = {0};
SystemStatus M2RStatus = {0};

SystemStatus* LocalStatus = 0;

static void process_packet(SystemStatus *status, TelemPacket *packet);
static void generate_packets(SystemStatus *status, void (*sender)(TelemPacket*));
static void generate_common_packets(SystemStatus *status, void (*sender)(TelemPacket*));
static void generate_m2fc_packets(SystemStatus *status, void (*sender)(TelemPacket*));
static void generate_m2r_packets(SystemStatus *status, void (*sender)(TelemPacket*));
static void update_system_status(void);

void m2rl_send_packet(TelemPacket* packet);
void m2serial_send_packet(TelemPacket* packet);

msg_t m2status_thread(void* arg)
{
    (void)arg;

    /* Set the origin field on all the local statuses */
    M2FCBodyStatus.origin = M2T_ORIGIN_M2FCBODY;
    M2FCNoseStatus.origin = M2T_ORIGIN_M2FCNOSE;
    M2RStatus.origin = M2T_ORIGIN_M2R;

    /* Ensure LocalStatus is pointing somewhere useful before we start. */
    while(LocalStatus == NULL) chThdSleepMilliseconds(10);

    while(true) {
        /* Generate packets from the SystemStatus structs and send them
         * elsewhere. Specifically...
         * M2FCBody:
         *  Send M2FCBody status to M2FC Nose via xbee for logging and relaying
         * M2FCNose:
         *  Send M2FCNose status to M2FC Body via xbee for logging
         *  Send M2R status to M2FC Body via xbee for logging
         *  Send M2FCNose status to M2R via M2Serial for relaying
         *  Send M2FCBody status to M2R via M2Serial for relaying
         * M2R:
         *  Send M2R status to M2FC Body via M2Serial for logging and relaying
         */

        if(LocalStatus == &M2FCBodyStatus) {
            /* Send our status to M2FC Nose */
            generate_packets(&M2FCBodyStatus, m2rl_send_packet);

        } else if(LocalStatus == &M2FCNoseStatus) {
            /* Send our status and M2R status to M2FC Body */
            generate_packets(&M2FCNoseStatus, m2rl_send_packet);
            generate_packets(&M2RStatus, m2rl_send_packet);

            /* Send our status and M2FC Body status to M2R */
            generate_packets(&M2FCBodyStatus, m2serial_send_packet);
            generate_packets(&M2FCNoseStatus, m2serial_send_packet);

        } else if(LocalStatus == &M2RStatus) {
            /* Send our status to M2FC Body */
            generate_packets(&M2RStatus, m2serial_send_packet);
        }
        
        /* Sleep for a second and do it all again. */
        chThdSleepMilliseconds(1000);
    }
}

static void generate_packets(SystemStatus *status,
                             void (*sender)(TelemPacket*))
{
    generate_common_packets(status, sender);
    if(status->origin == M2T_ORIGIN_M2FCBODY ||
       status->origin == M2T_ORIGIN_M2FCNOSE)
    {
        generate_m2fc_packets(status, sender);
    }
    else if(status->origin == M2T_ORIGIN_M2R)
    {
        generate_m2r_packets(status, sender);
    }
}

static void generate_common_packets(SystemStatus *status, void (*sender)(TelemPacket*))
{
    TelemPacket pkt;

    pkt.channel = M2T_CH_SYS_STATS;
    pkt.u8[0] = status->latest.cpu_usage;
    pkt.u8[1] = status->latest.microsd_duty;
    pkt.timestamp = status->latest_timestamps.stats;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_SYS_STATUS_1;
    pkt.u8[0] = status->m2fcbody;
    pkt.u8[1] = status->m2fcnose;
    pkt.u8[2] = status->m2r;
    pkt.timestamp = status->latest_timestamps.status_1;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_SYS_STATUS_2;
    pkt.u8[0] = status->adc;
    pkt.u8[1] = status->accel;
    pkt.u8[2] = status->baro;
    pkt.u8[3] = status->gyro;
    pkt.u8[4] = status->magno;
    pkt.u8[5] = status->pyro;
    pkt.u8[6] = status->microsd;
    pkt.timestamp = status->latest_timestamps.status_2;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_SYS_STATUS_3;
    pkt.u8[0] = status->stateestimation;
    pkt.u8[1] = status->missioncontrol;
    pkt.u8[2] = status->datalogging;
    pkt.u8[3] = status->config;
    pkt.timestamp = status->latest_timestamps.status_3;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_SYS_STATUS_4;
    pkt.u8[0] = status->rockblock;
    pkt.u8[1] = status->radio;
    pkt.u8[2] = status->gps;
    pkt.timestamp = status->latest_timestamps.status_4;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

}

static void generate_m2fc_packets(SystemStatus *status, void (*sender)(TelemPacket*))
{
    TelemPacket pkt;

    pkt.channel = M2T_CH_IMU_LG_ACCEL;
    pkt.i16[0] = status->latest.lga_x;
    pkt.i16[1] = status->latest.lga_y;
    pkt.i16[2] = status->latest.lga_z;
    pkt.timestamp = status->latest_timestamps.lga;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_IMU_HG_ACCEL;
    pkt.i16[0] = status->latest.hga_x;
    pkt.i16[1] = status->latest.hga_y;
    pkt.i16[2] = status->latest.hga_z;
    pkt.timestamp = status->latest_timestamps.hga;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_IMU_BARO;
    pkt.i32[0] = status->latest.baro_p;
    pkt.i32[1] = status->latest.baro_t;
    pkt.timestamp = status->latest_timestamps.baro;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_IMU_GYRO;
    pkt.i16[0] = status->latest.gyro_x;
    pkt.i16[1] = status->latest.gyro_y;
    pkt.i16[2] = status->latest.gyro_z;
    pkt.timestamp = status->latest_timestamps.gyro;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_IMU_MAGNO;
    pkt.i16[0] = status->latest.magno_x;
    pkt.i16[1] = status->latest.magno_y;
    pkt.i16[2] = status->latest.magno_z;
    pkt.timestamp = status->latest_timestamps.magno;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_ADC_STRAIN;
    pkt.i16[0] = status->latest.sg1;
    pkt.i16[1] = status->latest.sg2;
    pkt.i16[2] = status->latest.sg3;
    pkt.timestamp = status->latest_timestamps.sg;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_ADC_THERMO;
    pkt.i16[0] = status->latest.tc1;
    pkt.i16[1] = status->latest.tc2;
    pkt.i16[2] = status->latest.tc3;
    pkt.timestamp = status->latest_timestamps.tc;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_STATE_MISSION;
    pkt.i32[1] = status->latest.mc_state;
    pkt.timestamp = status->latest_timestamps.mc;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_SE_T_H;
    pkt.f[0] = status->latest.se_dt;
    pkt.f[1] = status->latest.se_h;
    pkt.timestamp = status->latest_timestamps.se_predict_1;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_SE_V_A;
    pkt.f[0] = status->latest.se_v;
    pkt.f[1] = status->latest.se_a;
    pkt.timestamp = status->latest_timestamps.se_predict_2;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_SE_PRESSURE;
    pkt.f[0] = status->latest.se_p_s;
    pkt.f[1] = status->latest.se_p_e;
    pkt.timestamp = status->latest_timestamps.se_pressure;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_SE_ACCEL;
    pkt.f[0] = status->latest.se_a_s;
    pkt.f[1] = status->latest.se_a_e;
    pkt.timestamp = status->latest_timestamps.se_accel;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);
        
    pkt.channel = M2T_CH_PYRO_CONT;
    pkt.i16[0] = status->latest.pyro_c_1;
    pkt.i16[1] = status->latest.pyro_c_2;
    pkt.i16[2] = status->latest.pyro_c_3;
    pkt.timestamp = status->latest_timestamps.pyro_c;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_PYRO_FIRE;
    pkt.i16[0] = status->latest.pyro_f_1;
    pkt.i16[1] = status->latest.pyro_f_2;
    pkt.i16[2] = status->latest.pyro_f_3;
    pkt.timestamp = status->latest_timestamps.pyro_f;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

}

static void generate_m2r_packets(SystemStatus *status, void (*sender)(TelemPacket*))
{
    TelemPacket pkt;

    pkt.channel = M2T_CH_ADC_BATT;
    pkt.i16[0] = status->latest.battery;
    pkt.timestamp = status->latest_timestamps.battery;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_GPS_TIME;
    pkt.u8[0] = status->latest.gps_t_year_l;
    pkt.u8[1] = status->latest.gps_t_year_h;
    pkt.u8[2] = status->latest.gps_t_month;
    pkt.u8[3] = status->latest.gps_t_day;
    pkt.u8[4] = status->latest.gps_t_hour;
    pkt.u8[5] = status->latest.gps_t_min;
    pkt.u8[6] = status->latest.gps_t_sec;
    pkt.u8[7] = status->latest.gps_t_valid;
    pkt.timestamp = status->latest_timestamps.gps_t;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_GPS_POS;
    pkt.i32[0] = status->latest.gps_lat;
    pkt.i32[1] = status->latest.gps_lng;
    pkt.timestamp = status->latest_timestamps.gps_p;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_GPS_ALT;
    pkt.i32[0] = status->latest.gps_alt;
    pkt.i32[1] = status->latest.gps_alt_msl;
    pkt.timestamp = status->latest_timestamps.gps_a;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);

    pkt.channel = M2T_CH_GPS_STATUS;
    pkt.i8[0] = status->latest.gps_fix_type;
    pkt.i8[1] = status->latest.gps_flags;
    pkt.i8[2] = status->latest.gps_num_sv;
    pkt.timestamp = status->latest_timestamps.gps_s;
    pkt.metadata = status->origin;
    m2telem_write_checksum(&pkt);
    sender(&pkt);
}

void m2status_rx_packet(TelemPacket *packet)
{
    /*
     * Update one of the SystemStatus structs based on this new packet we've
     * received from somewhere.
     */
    if((packet->metadata & 0xF) == M2T_ORIGIN_M2FCBODY) {
        process_packet(&M2FCBodyStatus, packet);
    } else if((packet->metadata & 0xF) == M2T_ORIGIN_M2FCNOSE) {
        process_packet(&M2FCNoseStatus, packet);
    } else if((packet->metadata & 0xF) == M2T_ORIGIN_M2R) {
        process_packet(&M2RStatus, packet);
    }
}

static void process_packet(SystemStatus *status, TelemPacket *packet)
{
    switch(packet->channel) {
        case M2T_CH_SYS_STATS:
            status->latest.cpu_usage = packet->u8[0];
            status->latest.microsd_duty = packet->u8[1];
            status->latest_timestamps.stats = packet->timestamp;
            break;

        case M2T_CH_SYS_STATUS_1:
            status->m2fcbody = packet->u8[0];
            status->m2fcnose = packet->u8[1];
            status->m2r = packet->u8[2];
            status->latest_timestamps.status_1 = packet->timestamp;
            break;

        case M2T_CH_SYS_STATUS_2:
            status->adc = packet->u8[0];
            status->accel = packet->u8[1];
            status->baro = packet->u8[2];
            status->gyro = packet->u8[3];
            status->magno = packet->u8[4];
            status->pyro = packet->u8[5];
            status->microsd = packet->u8[6];
            status->latest_timestamps.status_2 = packet->timestamp;
            break;

        case M2T_CH_SYS_STATUS_3:
            status->stateestimation = packet->u8[0];
            status->missioncontrol = packet->u8[1];
            status->datalogging = packet->u8[2];
            status->config = packet->u8[3];
            status->latest_timestamps.status_3 = packet->timestamp;
            break;

        case M2T_CH_SYS_STATUS_4:
            status->rockblock = packet->u8[0];
            status->radio = packet->u8[1];
            status->gps = packet->u8[2];
            status->latest_timestamps.status_4 = packet->timestamp;
            break;

        case M2T_CH_IMU_LG_ACCEL:
            status->latest.lga_x = packet->i16[0];
            status->latest.lga_y = packet->i16[1];
            status->latest.lga_z = packet->i16[2];
            status->latest_timestamps.lga = packet->timestamp;
            break;

        case M2T_CH_IMU_HG_ACCEL:
            status->latest.hga_x = packet->i16[0];
            status->latest.hga_y = packet->i16[1];
            status->latest.hga_z = packet->i16[2];
            status->latest_timestamps.hga = packet->timestamp;
            break;

        case M2T_CH_IMU_BARO:
            status->latest.baro_p = packet->i32[0];
            status->latest.baro_t = packet->i32[1];
            status->latest_timestamps.baro = packet->timestamp;
            break;

        case M2T_CH_IMU_GYRO:
            status->latest.gyro_x = packet->i16[0];
            status->latest.gyro_y = packet->i16[1];
            status->latest.gyro_z = packet->i16[2];
            status->latest_timestamps.gyro = packet->timestamp;
            break;

        case M2T_CH_IMU_MAGNO:
            status->latest.magno_x = packet->i16[0];
            status->latest.magno_y = packet->i16[1];
            status->latest.magno_z = packet->i16[2];
            status->latest_timestamps.magno = packet->timestamp;
            break;

        case M2T_CH_ADC_BATT:
            status->latest.battery = packet->i16[0];
            status->latest_timestamps.battery = packet->timestamp;
            break;

        case M2T_CH_ADC_STRAIN:
            status->latest.sg1 = packet->i16[0];
            status->latest.sg2 = packet->i16[1];
            status->latest.sg3 = packet->i16[2];
            status->latest_timestamps.sg = packet->timestamp;
            break;

        case M2T_CH_ADC_THERMO:
            status->latest.tc1 = packet->i16[0];
            status->latest.tc2 = packet->i16[1];
            status->latest.tc3 = packet->i16[2];
            status->latest_timestamps.tc = packet->timestamp;
            break;

        case M2T_CH_STATE_MISSION:
            status->latest.mc_state = packet->i32[1];
            status->latest_timestamps.mc = packet->timestamp;
            break;

        case M2T_CH_SE_T_H:
            status->latest.se_dt = packet->f[0];
            status->latest.se_h = packet->f[1];
            status->latest_timestamps.se_predict_1 = packet->timestamp;
            break;

        case M2T_CH_SE_V_A:
            status->latest.se_v = packet->f[0];
            status->latest.se_a = packet->f[1];
            status->latest_timestamps.se_predict_2 = packet->timestamp;
            break;

        case M2T_CH_SE_PRESSURE:
            status->latest.se_p_s = packet->f[0];
            status->latest.se_p_e = packet->f[1];
            status->latest_timestamps.se_pressure = packet->timestamp;
            break;

        case M2T_CH_SE_ACCEL:
            status->latest.se_a_s = packet->f[0];
            status->latest.se_a_e = packet->f[1];
            status->latest_timestamps.se_accel = packet->timestamp;
            break;
        
        case M2T_CH_PYRO_CONT:
            status->latest.pyro_c_1 = packet->i16[0];
            status->latest.pyro_c_2 = packet->i16[1];
            status->latest.pyro_c_3 = packet->i16[2];
            status->latest_timestamps.pyro_c = packet->timestamp;
            break;

        case M2T_CH_PYRO_FIRE:
            status->latest.pyro_f_1 = packet->i16[0];
            status->latest.pyro_f_2 = packet->i16[1];
            status->latest.pyro_f_3 = packet->i16[2];
            status->latest_timestamps.pyro_f = packet->timestamp;
            break;

        case M2T_CH_GPS_TIME:
            status->latest.gps_t_year_l = packet->u8[0];
            status->latest.gps_t_year_h = packet->u8[1];
            status->latest.gps_t_month = packet->u8[2];
            status->latest.gps_t_day = packet->u8[3];
            status->latest.gps_t_hour = packet->u8[4];
            status->latest.gps_t_min = packet->u8[5];
            status->latest.gps_t_sec = packet->u8[6];
            status->latest.gps_t_valid = packet->u8[7];
            status->latest_timestamps.gps_t = packet->timestamp;
            break;

        case M2T_CH_GPS_POS:
            status->latest.gps_lat = packet->i32[0];
            status->latest.gps_lng = packet->i32[1];
            status->latest_timestamps.gps_p = packet->timestamp;
            break;

        case M2T_CH_GPS_ALT:
            status->latest.gps_alt = packet->i32[0];
            status->latest.gps_alt_msl = packet->i32[1];
            status->latest_timestamps.gps_a = packet->timestamp;
            break;

        case M2T_CH_GPS_STATUS:
            status->latest.gps_fix_type = packet->i8[0];
            status->latest.gps_flags = packet->i8[1];
            status->latest.gps_num_sv = packet->i8[2];
            status->latest_timestamps.gps_s = packet->timestamp;
            break;
    }
}

void m2status_adc_status(Status status)
{
    LocalStatus->adc = status;
    update_system_status();
}

void m2status_accel_status(Status status)
{
    LocalStatus->accel = status;
    update_system_status();
}

void m2status_baro_status(Status status)
{
    LocalStatus->baro = status;
    update_system_status();
}

void m2status_gyro_status(Status status)
{
    LocalStatus->gyro = status;
    update_system_status();
}

void m2status_magno_status(Status status)
{
    LocalStatus->magno = status;
    update_system_status();
}

void m2status_pyro_status(Status status)
{
    LocalStatus->pyro = status;
    update_system_status();
}

void m2status_microsd_status(Status status)
{
    LocalStatus->microsd = status;
    update_system_status();
}

void m2status_stateestimation_status(Status status)
{
    LocalStatus->stateestimation = status;
    update_system_status();
}

void m2status_missioncontrol_status(Status status)
{
    LocalStatus->missioncontrol = status;
    update_system_status();
}

void m2status_datalogging_status(Status status)
{
    LocalStatus->datalogging = status;
    update_system_status();
}

void m2status_config_status(Status status)
{
    LocalStatus->config = status;
    update_system_status();
}

void m2status_rockblock_status(Status status)
{
    LocalStatus->rockblock = status;
    update_system_status();
}

void m2status_radio_status(Status status)
{
    LocalStatus->radio = status;
    update_system_status();
}

void m2status_gps_status(Status status)
{
    LocalStatus->gps = status;
    update_system_status();
}

/* Update LocalStatus->m2{fcbody,fcnose,r} based on the relevant subsystems'
 * status.
 */
static void update_system_status()
{
    Status* mystatus;

    if(LocalStatus->origin == M2T_ORIGIN_M2FCBODY ||
       LocalStatus->origin == M2T_ORIGIN_M2FCNOSE)
    {
        if(LocalStatus->origin == M2T_ORIGIN_M2FCBODY) {
            mystatus = &(LocalStatus->m2fcbody);
        } else if(LocalStatus->origin == M2T_ORIGIN_M2FCNOSE) {
            mystatus = &(LocalStatus->m2fcnose);
        }

        *mystatus = STATUS_OK;

        if(
           LocalStatus->adc == STATUS_UNKNOWN ||
           LocalStatus->accel == STATUS_UNKNOWN ||
           LocalStatus->baro == STATUS_UNKNOWN ||
           LocalStatus->gyro == STATUS_UNKNOWN ||
           LocalStatus->magno == STATUS_UNKNOWN ||
           LocalStatus->pyro == STATUS_UNKNOWN ||
           LocalStatus->microsd == STATUS_UNKNOWN ||
           LocalStatus->stateestimation == STATUS_UNKNOWN ||
           LocalStatus->missioncontrol == STATUS_UNKNOWN ||
           LocalStatus->datalogging == STATUS_UNKNOWN ||
           LocalStatus->config == STATUS_UNKNOWN
          )
        {
            *mystatus = STATUS_UNKNOWN;
        }

        if(
           LocalStatus->adc == STATUS_WAIT ||
           LocalStatus->accel == STATUS_WAIT ||
           LocalStatus->baro == STATUS_WAIT ||
           LocalStatus->gyro == STATUS_WAIT ||
           LocalStatus->magno == STATUS_WAIT ||
           LocalStatus->pyro == STATUS_WAIT ||
           LocalStatus->microsd == STATUS_WAIT ||
           LocalStatus->stateestimation == STATUS_WAIT ||
           LocalStatus->missioncontrol == STATUS_WAIT ||
           LocalStatus->datalogging == STATUS_WAIT ||
           LocalStatus->config == STATUS_WAIT
          )
        {
            *mystatus = STATUS_WAIT;
        }

        if(
           LocalStatus->adc >= STATUS_ERR ||
           LocalStatus->accel >= STATUS_ERR ||
           LocalStatus->baro >= STATUS_ERR ||
           LocalStatus->gyro >= STATUS_ERR ||
           LocalStatus->magno >= STATUS_ERR ||
           LocalStatus->pyro >= STATUS_ERR ||
           LocalStatus->microsd >= STATUS_ERR ||
           LocalStatus->stateestimation >= STATUS_ERR ||
           LocalStatus->missioncontrol >= STATUS_ERR ||
           LocalStatus->datalogging >= STATUS_ERR ||
           LocalStatus->config >= STATUS_ERR
          )
        {
            *mystatus = STATUS_ERR;
        }

    } else if(LocalStatus->origin == M2T_ORIGIN_M2R) {
        if(LocalStatus->origin == M2T_ORIGIN_M2R) {
            mystatus = &(LocalStatus->m2r);
        }

        *mystatus = STATUS_OK;

        if(
           LocalStatus->rockblock == STATUS_UNKNOWN ||
           LocalStatus->radio == STATUS_UNKNOWN ||
           LocalStatus->gps == STATUS_UNKNOWN
          )
        {
            *mystatus = STATUS_UNKNOWN;
        }

        if(
           LocalStatus->rockblock == STATUS_WAIT ||
           LocalStatus->radio == STATUS_WAIT ||
           LocalStatus->gps == STATUS_WAIT
          )
        {
            *mystatus = STATUS_WAIT;
        }

        if(
           LocalStatus->rockblock >= STATUS_ERR ||
           LocalStatus->radio >= STATUS_ERR ||
           LocalStatus->gps >= STATUS_ERR
          )
        {
            *mystatus = STATUS_ERR;
        }
    }
}

void m2status_set_sg(int16_t sg1, int16_t sg2, int16_t sg3)
{
    LocalStatus->latest_timestamps.sg = halGetCounterValue();
    LocalStatus->latest.sg1 = sg1;
    LocalStatus->latest.sg2 = sg2;
    LocalStatus->latest.sg3 = sg3;
}

void m2status_set_tc(int16_t tc1, int16_t tc2, int16_t tc3)
{
    LocalStatus->latest_timestamps.tc = halGetCounterValue();
    LocalStatus->latest.tc1 = tc1;
    LocalStatus->latest.tc2 = tc2;
    LocalStatus->latest.tc3 = tc3;
}

void m2status_set_lga(int16_t x, int16_t y, int16_t z)
{
    LocalStatus->latest_timestamps.lga = halGetCounterValue();
    LocalStatus->latest.lga_x = x;
    LocalStatus->latest.lga_y = y;
    LocalStatus->latest.lga_z = z;
}

void m2status_set_hga(int16_t x, int16_t y, int16_t z)
{
    LocalStatus->latest_timestamps.hga = halGetCounterValue();
    LocalStatus->latest.hga_x = x;
    LocalStatus->latest.hga_y = y;
    LocalStatus->latest.hga_z = z;
}

void m2status_set_baro(int32_t p, int32_t t)
{
    LocalStatus->latest_timestamps.baro = halGetCounterValue();
    LocalStatus->latest.baro_p = p;
    LocalStatus->latest.baro_t = t;
}

void m2status_set_gyro(int16_t x, int16_t y, int16_t z)
{
    LocalStatus->latest_timestamps.gyro = halGetCounterValue();
    LocalStatus->latest.gyro_x = x;
    LocalStatus->latest.gyro_y = y;
    LocalStatus->latest.gyro_z = z;
}

void m2status_set_magno(int16_t x, int16_t y, int16_t z)
{
    LocalStatus->latest_timestamps.magno = halGetCounterValue();
    LocalStatus->latest.magno_x = x;
    LocalStatus->latest.magno_y = y;
    LocalStatus->latest.magno_z = z;
}

void m2status_set_se_pred(float dt, float h, float v, float a)
{
    LocalStatus->latest_timestamps.se_predict_1 = halGetCounterValue();
    LocalStatus->latest_timestamps.se_predict_2 = halGetCounterValue();
    LocalStatus->latest.se_dt = dt;
    LocalStatus->latest.se_h = h;
    LocalStatus->latest.se_v = v;
    LocalStatus->latest.se_a = a;
}

void m2status_set_se_pressure(float sensor, float estimate)
{
    LocalStatus->latest_timestamps.se_pressure = halGetCounterValue();
    LocalStatus->latest.se_p_s = sensor;
    LocalStatus->latest.se_p_e = estimate;
}

void m2status_set_se_accel(float sensor, float estimate)
{
    LocalStatus->latest_timestamps.se_accel = halGetCounterValue();
    LocalStatus->latest.se_a_s = sensor;
    LocalStatus->latest.se_a_e = estimate;
}

void m2status_set_mc(int32_t state)
{
    LocalStatus->latest_timestamps.mc = halGetCounterValue();
    LocalStatus->latest.mc_state = state;
}

void m2status_set_pyro_c(int16_t py1, int16_t py2, int16_t py3)
{
    LocalStatus->latest_timestamps.pyro_c = halGetCounterValue();
    LocalStatus->latest.pyro_c_1 = py1;
    LocalStatus->latest.pyro_c_2 = py2;
    LocalStatus->latest.pyro_c_3 = py3;
}

void m2status_set_pyro_f(int16_t py1, int16_t py2, int16_t py3)
{
    LocalStatus->latest_timestamps.pyro_f = halGetCounterValue();
    LocalStatus->latest.pyro_f_1 = py1;
    LocalStatus->latest.pyro_f_2 = py2;
    LocalStatus->latest.pyro_f_3 = py3;
}

void m2status_set_battery(int16_t battery_mv)
{
    LocalStatus->latest_timestamps.battery = halGetCounterValue();
    LocalStatus->latest.battery = battery_mv;
}

void m2status_set_gps_time(uint8_t year_l, uint8_t year_h, uint8_t month,
                           uint8_t day, uint8_t hour, uint8_t min, uint8_t sec,
                           uint8_t valid)
{
    LocalStatus->latest_timestamps.gps_t = halGetCounterValue();
    LocalStatus->latest.gps_t_year_l = year_l;
    LocalStatus->latest.gps_t_year_h = year_h;
    LocalStatus->latest.gps_t_month = month;
    LocalStatus->latest.gps_t_day = day;
    LocalStatus->latest.gps_t_hour = hour;
    LocalStatus->latest.gps_t_min = min;
    LocalStatus->latest.gps_t_sec = sec;
    LocalStatus->latest.gps_t_valid = valid;
}

void m2status_set_gps_pos(int32_t lat, int32_t lng)
{
    LocalStatus->latest_timestamps.gps_p = halGetCounterValue();
    LocalStatus->latest.gps_lat = lat;
    LocalStatus->latest.gps_lng = lng;
}

void m2status_set_gps_alt(int32_t alt, int32_t alt_msl)
{
    LocalStatus->latest_timestamps.gps_a = halGetCounterValue();
    LocalStatus->latest.gps_alt = alt;
    LocalStatus->latest.gps_alt_msl = alt_msl;
}

void m2status_set_gps_status(int8_t fix_type, int8_t flags, int8_t num_sv)
{
    LocalStatus->latest_timestamps.gps_s = halGetCounterValue();
    LocalStatus->latest.gps_fix_type = fix_type;
    LocalStatus->latest.gps_flags = flags;
    LocalStatus->latest.gps_num_sv = num_sv;
}


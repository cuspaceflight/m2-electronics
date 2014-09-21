/*
 * Mission configuration file
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Velocity (m/s) beyond which the rocket has left the pad */
#define IGNITION_VELOCITY 10.0f

/* Acceleration (m/s/s) below which the motor has ceased burning */
#define BURNOUT_ACCELERATION 2.0f

/* Time (ms) since launch beyond which the motor has ceased burning */
#define BURNOUT_TIMER 5200

/* Time (ms) since launch beyond which apogee has been reached */
#define APOGEE_TIMER 55000

/* Altitude (m ASL) below which to deploy main chute */
#define MAIN_DEPLOY_ALTITUDE 1450.0f

/* Time (ms) since apogee beyond which to deploy the main chute */
#define MAIN_DEPLOY_TIMER 30000

/* Time (ms) since apogee after which the rocket has landed */
#define LANDED_TIMER 300000

/* Duration (ms) to fire pyros for */
#define PYRO_FIRETIME 5000

/* Which flight computer should fire which pyros when? */
#define PYRO_DROGUE_BODY_1 FALSE
#define PYRO_DROGUE_BODY_2 FALSE
#define PYRO_DROGUE_BODY_3 FALSE
#define PYRO_DROGUE_NOSE_1 TRUE
#define PYRO_DROGUE_NOSE_2 TRUE
#define PYRO_DROGUE_NOSE_3 TRUE
#define PYRO_MAIN_BODY_1   FALSE
#define PYRO_MAIN_BODY_2   FALSE
#define PYRO_MAIN_BODY_3   TRUE
#define PYRO_MAIN_NOSE_1   FALSE
#define PYRO_MAIN_NOSE_2   FALSE
#define PYRO_MAIN_NOSE_3   FALSE

/* Thrust axis for the two accelerometers.
 * 0 - X
 * 1 - Y
 * 2 - Z
 */
#define ACCEL_THRUST_AXIS 2


/* END OF CONFIGURATION OPTIONS */

typedef enum { M2FC_BODY=1, M2FC_NOSE=2, M2R=3 } config_avionics_t;
extern config_avionics_t m2fc_location;
void config_read_location(void);

#endif /* CONFIG_H */

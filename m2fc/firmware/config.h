/*
 * Mission configuration file
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Duration (ms) to fire pyros for */
#define PYRO_FIRETIME 3000

/* Velocity (m/s) beyond which the rocket has left the pad */
#define IGNITION_VELOCITY 10.0f

/* Acceleration (m/s/s) below which the motor has ceased burning */
#define BURNOUT_ACCELERATION 2.0f

/* Time (ms) since launch beyond which the motor has ceased burning */
#define BURNOUT_TIMER 5000

/* Acceleration (m/s/s) below which apogee has been reached */
#define APOGEE_ACCELERATION 0.3f

/* Time (ms) since launch beyond which apogee has been reached */
#define APOGEE_TIMER 15000

/* Altitude (m ASL) below which to deploy main chute */
#define MAIN_DEPLOY_ALTITUDE 250.0f

/* Time (ms) since apogee beyond which to deploy the main chute */
#define MAIN_DEPLOY_TIMER 20000

/* Velocity (m/s) below which the rocket has landed */
#define LANDED_VELOCITY 3.0f

/* Time (ms) since apogee after which the rocket has landed */
#define LANDED_TIMER 180000

#endif /* CONFIG_H */

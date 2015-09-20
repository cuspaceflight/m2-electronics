# Martlet 2 Config Files

Configuration settings are saved in a file named `config.txt` in the root of 
the SD card. Each line is one `key=value` pair, with values interpreted as 
either integers or floating point numbers. Bools should be specified as 0 or 1. 
Ints are all unsigned (in other words, non-negative, >= 0).

Configuration keys must be *in order* and exactly as follows:


Key              | Type  | Description
-----------------|-------|-----------------------------------------------------
accel_axis       | Int   | Which accelerometer axis is 'up', 0=x 1=y 2=z
pyro_firetime    | Int   | Time in milliseconds to fire pyros
pyro_1           | Int   | Pyro channel 1 usage: 0=unused 1=drogue 2=main
pyro_2           | Int   | Pyro channel 2 usage: 0=unused 1=drogue 2=main
pyro_3           | Int   | Pyro channel 3 usage: 0=unused 1=drogue 2=main
ignition_accel   | Int   | Acceleration threshold for detecting motor ignition, in m/s/s
burnout_time     | Int   | Time limit on motor burn duration, in milliseconds
apogee_time      | Int   | Time since burnout after which apogee is considered to have been reached, in milliseconds
main_altitude    | Int   | Altitude below which to release the main parachute, in metres AGL
main_time        | Int   | Time since apogee at which to release the main parachute, in milliseconds
landing_time     | Int   | Time since apogee at which to consider the rocket landed, in milliseconds
use_adc          | Bool  | 1 to read the ADCs, 0 to disable
use_magno        | Bool  | 1 to read the magno, 0 to disable
use_gyro         | Bool  | 1 to read the gyro, 0 to disable

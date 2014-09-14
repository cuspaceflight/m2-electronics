# Martet 2 Telemetry

## Packet Format

16 byte packets: 8 byte header, 8 byte data.

    [  0  | TIMESTAMP  ]\
    [  1  | TIMESTAMP  ] |- A 32 bit unsigned timestamp that runs at the system
    [  2  | TIMESTAMP  ] |  clock frequency so will overflow fairly often.
    [  3  | TIMESTAMP  ]/
    [  4  | METADATA   ]--- Origin and format of data.
    [  5  | CHANNEL    ]--- Data channel.
    [  6  | CHECKSUM   ]\ 
    [  7  | CHECKSUM   ]/-- Validates the packet, normal CRC16-CCCITT.
    [  8  | DATA       ]\
    [  9  | DATA       ] |
    [ 10  | DATA       ] |
    [ 11  | DATA       ] |- User data. Might be an 8-char string or an int64 or
    [ 12  | DATA       ] |  two floats or four int16s or whatever else MODE
    [ 13  | DATA       ] |  indicates.
    [ 14  | DATA       ] |
    [ 15  | DATA       ]/

## Metadata

The metadata byte is split into two four-bit sections, the origin (first four
bits) and the mode (last four bits).

### Packet Origins

    0   RESERVED
    1   M2FC BODY
    2   M2FC NOSE
    3   M2R

### Packet Modes

    Mode  Type      Format   Description
    -------------------------------------------------------------
     0    char*      s8       Eight byte string
     1    int64_t    q        64 bit signed integer
     2    uint64_t   Q        64 bit unsigned integer
     3    int32_t    ii       Two 32bit signed integers
     4    uint32_t   II       Two 32bit unsigned integers
     5    int16_t    hhhh     Four 16bit signed integers
     6    uint16_t   HHHH     Four 16bit unsigned integers
     7    int8_t     bbbbbbbb Eight 8bit signed integers
     8    uint8_t    BBBBBBBB Eight 8bit unsigned integers
     9    float      ff       Two 32bit single precision floats
     10   double     d        One 64bit double precision float

## Channel

    Channel  Mode  Description
    -------------------------------------------------------------
    0x0            SYSTEM
       0     0      Initialisation message, contains the origin string


    0x1           CALIBRATION
       0     4      Timestamp frequency  [f 0]
       1     5      Low-G accelerometer  [axis gravity-magnitude 0 0]
       2     5      High-G accelerometer [axis gravity-magnitude 0 0]
       3     6      Barometer            [d0 c1 c2 c3]
       4     6      Barometer            [c4 c5 c6 d7]


    0x2           INTERTIAL MEASUREMENT
       0     5      Low-G accelerometer  [x y z 0]
       1     5      High-G accelerometer [x y z 0]
       2     3      Barometer            [pressure temperature]
       3     -      Rate gyroscope       [x y z 0]
       4     -      Magnetometer         [x y z 0]


    0x3           EXTERNAL SENSORS
       0     3      Battery voltage [millivolts 0]
       1     5      Strain gauges   [ch1 ch2 ch3 0]
       2     5      Thermocouples   [ch1 ch2 ch3 0]

    0x4           STATE MACHINES
       0     3      M2FC Mission Control [old_state new_state]

    0x5           STATE ESTIMATION
       0     9      Prediction output  I     [dt       height]
       1     9      Prediction output II     [velocity acceleration]
       2     9      Pressure measurement     [sensor   estimate]
       3     9      Acceleration measurement [sensor   estimate]

    0x6           PYROTECHNICS
       0     5      Continuity results [ch1 ch2 ch3 0]
       1     5      Fired              [ch1 ch2 ch3 0]

TODO: GPS fixes, RockBLOCK messages.

## Checksum

CRC16-CCITT with polynomial 0x1021 and initial value 0x0000.

## Data

Data in whatever format is described by the mode in the metadata field.
Multi-byte entries are stored little-endian.

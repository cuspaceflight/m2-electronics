# Martet 2 Telemetry

Document version: 2.0

This document describes version 2 of the Martlet 2 telemetry packet format. 
Compared to version 1, the two halves (the data segment and the metadata 
segment) are switched, to accommodate a trailing checksum. The packet mode 
became _managed_, associated with a particular channel, rather than being 
specified in the packet itself. Origins and channel constants are unchanged.

## Packet Format

16 byte packets: 8 byte data, 8 byte footer.

    [  0  | DATA       ]\
    [  1  | DATA       ] |
    [  2  | DATA       ] |
    [  3  | DATA       ] |- User data. Might be an 8-char string or an int64 or
    [  4  | DATA       ] |  two floats or four int16s or whatever else MODE
    [  5  | DATA       ] |  indicates.
    [  6  | DATA       ] |
    [  7  | DATA       ]/
    [  8  | TIMESTAMP  ]\
    [  9  | TIMESTAMP  ] |- A 32 bit unsigned timestamp that runs at the system
    [ 10  | TIMESTAMP  ] |  clock frequency so will overflow fairly often.
    [ 11  | TIMESTAMP  ]/
    [ 12  | METADATA   ]--- Origin of data and reserved bits.
    [ 13  | CHANNEL    ]--- Data channel.
    [ 14  | CHECKSUM L ]\
    [ 15  | CHECKSUM U ]/-- Verify packet contents.

## Metadata

The metadata byte describes the original source of a telemetry packet in the 
lower four bits. The upper four bits are reserved.

### Packet Origins

    0   RESERVED
    1   M2FC BODY
    2   M2FC NOSE
    3   M2R
    A   Groundstation

## Channel

The channel byte describes what data this packet contains. The `mode`
referenced in the channel table refers to one of the data formats described in
the `Data` section.

### Packet Channels

    Channel  Mode  Description
    -------------------------------------------------------------
    0x0            SYSTEM
       0     0      Initialisation message, contains the origin string
       1     0      Firmware version, as Git commit hash


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
       3     5      Rate gyroscope       [x y z 0]
       4     5      Magnetometer         [x y z 0]


    0x3           EXTERNAL SENSORS
       0     3      Battery voltage [millivolts 0]
       1     5      Strain gauges   [ch1 ch2 ch3 0]
       2     5      Thermocouples   [ch1 ch2 ch3 0]

    0x4           STATE MACHINES
       0     3      Mission Control [old_state new_state]

    0x5           STATE ESTIMATION
       0     9      Prediction output  I     [dt       height]
       1     9      Prediction output II     [velocity acceleration]
       2     9      Pressure measurement     [sensor   estimate]
       3     9      Acceleration measurement [sensor   estimate]

    0x6           PYROTECHNICS
       0     5      Continuity results [ch1 ch2 ch3 0]
       1     5      Fired              [ch1 ch2 ch3 0]

    0x7           LOCATION
       0     8      GPS Time       [year_l year_h month day hour min sec valid]
       1     3      GPS Position   [latitude longitude]
       2     3      GPS Altitude   [height height_msl]
       3     7      GPS Status     [fix_type flags num_sv 0 0 0 0 0]

## Checksum

CRC16-CCITT with polynomial 0x1021 and initial value 0xFFFF, no 
post-processing. Stored little-endien.

## Data

Data formatting depends on channel, allocated in the channel table. Multi byte 
entries are stored little-endien.

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


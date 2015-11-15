---
title: Martlet II Avionics Postmortem
author: Adam Greig, CU Spaceflight
date: October 2015

header-includes:
    - \usepackage{framed,color}
    - \definecolor{shadecolor}{rgb}{0.639,0.757,0.678}
    - \usepackage{fancyhdr}
    - \pagestyle{fancy}
    - \fancyhead[L]{}
    - \fancyhead[C]{Martlet II Avionics Postmortem}
    - \fancyhead[R]{\thepage}
    - \fancyfoot[R]{}
    - \fancyfoot[C]{}
---

\fancypagestyle{plain}{
\renewcommand{\headrulewidth}{0pt}
\fancyhf{}
\fancyhead[L]{\includegraphics[width=3cm]{cusf_logo}}
\fancyhead[R]{\includegraphics[width=3cm]{m2_logo}}
\setlength\headheight{50pt}
}
![](avionics_set.jpg)

\clearpage

\setcounter{page}{1}

# Summary

## Overview

## Successes

## Problems

## Takeaways

\clearpage

\tableofcontents

# Introduction

## Martlet II Overview

Martlet 2 was an amateur rocket designed, built, and launched by Cambridge 
University Spaceflight in 2013-2015. It was intended to reach a speed of around 
1km/s and an altitude of around 15km, which would represent a new UK amateur 
record.

After a scrubbed launch due to weather in October 2014, Martlet 2 was launched 
in October 2015 from Black Rock Desert, Nevada, USA. An anomaly occurred three 
seconds into the flight, resulting in the rapid and thorough destruction of the 
rocket including the avionics. At this point the rocket is estimated to have 
reached an altitude of approximately 2km above ground level, and a velocity of 
approximately 1km/s. Much of the rocket structure and pieces of the avionics 
were recovered from the launch site.

![Martlet 2 in Black Rock Desert, Nevada, in October 2014](m2_2014.jpg)

## Report Scope and Goals

A custom set of avionics was designed for Martlet 2. After the scrubbed launch, 
some were updated to address issues in the original designs. This document is a 
postmortem and a retrospective of those avionics, reviewing the original design 
and evaluating the choices made and lessons learnt for future avionics 
programmes.

This postmortem is concerned with the avionics and their integration with the 
larger rocket system, but will not discuss matters solely related to the rocket 
or other components, nor will it detail the flight anomaly except insofar as it 
relates to the avionics design and possibility for future improvements.

## Project Objectives

The original objectives for the avionics were, in priority order:

1. Control the recovery system deployment
1. Downlink rocket position information for recovery purposes
1. Record vital rocket parameters (altitude, velocity, acceleration, location)
1. Record additional sensors, including inertial, strain, and temperature
1. Downlink additional sensor readings and rocket state information

\begin{shaded}
Think carefully about precise objectives and priority. Lots of time was wasted 
on work that was not mission critical to the detriment of things that were.
\end{shaded}

# Avionics Overview

This section describes the hardware as-flown to give context to the rest of the 
postmortem.

## Custom Hardware

### M2FC

`M2FC` is the main flight computer. Onboard is an `STM32F405VGT6` 
microcontroller, a 32-bit, high performance ARM device. There is a MicroSD 
card; an inertial measurement unit (IMU) consisting of a low-G accelerometer 
(`ADXL345`), high-G accelerometer (`ADXL375`), gyroscope (`L3G4200D`), 
magnetometer (`HMC5883L`), and barometer (`MS5611`); three pyrotechnic 
channels; three strain gauge analogue inputs; three thermocouple analogue 
inputs; and two isolated serial ports. Additionally there are connectors for 
the main power supply battery, an arming switch (which is simply placed inline 
with the battery before power is provided to the board), an auxiliary power 
output, and a bicolour status LED.

There are two `M2FC`s, one in the nosecone and one on the body tube mounting 
plate.

### M2R

`M2R` is the radio board. Onboard is an `STM32F303CBT7` microcontroller, a 
uBlox `MAX-M8Q` GPS receiver, a Radiometrix `MTX2` FM radio transmitter, and a 
buzzer. There are connectors for power input, a status LED, two isolated serial 
ports (which attach to `M2FC` and the RockBLOCK satellite modem), and isolated 
access to the GPSs' serial port.

There is one `M2R`, in the nosecone of the rocket.

### M2RL

`M2RL` is a radio link between the two `M2FCs`. It consists of a series 2 Xbee 
modem and serial isolating hardware.

### M2PA

`M2PA` is a radio power amplifier, boosting the 10mW from `M2R` to around 500mW 
for transmission.

### JOEY

`JOEY` is a radio tracker designed for high altitude ballooning use. It has an 
onboard radio and GPS receiver and simply transmits the current GPS location. 
One `JOEY` board was installed in the parachute shroud as a backup locator.

## Other Electronics

Additionally there are two small cameras, a RockBLOCK satellite modem, two 
arming switches, three battery boxes, and four Metron protractors.

Installed in the fin are three strain gauges in a half-bridge configuration, 
with an unloaded set of strain gauges to provide temperature compensation, and 
a thermocouple to measure the fin root temperature.

### RockBLOCK

RockBLOCKs are Iridium satellite modems, capable of sending and receiving short 
messages anywhere in the world. One RockBLOCK was installed as a backup 
communication method in case the main radio link failed.

### PerfectFlite

A PerfectFlite commercial altimeter was installed on the body tube mounting 
plate to record the maximum altitude reached for official record-keeping 
purposes.

### Cameras

Two "808" miniature video cameras were installed, one in the nosecone looking 
downwards and one on the body tube mounting plate looking out into a 
downwards-facing mirror.

## Other Components

### Metrons

Metron protractors are pyrotechnic devices which, upon activation, displace a 
metal pin with significant force. Three are used to physically push the 
nosecone away from the rocket body, which causes the drogue parachute to be 
released, while one is used on the body tube mounting plate to release the main 
parachute.

TODO: Metron model

### Strain Gauges

Two units of a three-gauge rosette were mounted in one fin root, aligned with 
the flight axis, 45 degrees to the axis, and 90 degrees to the axis. The second 
unit was unloaded to provide temperature compensation.

TODO: Strain gauge model

### Thermocouples

A K-type thermocouple was installed in the fin root to record its temperature 
and that of the adjacent strain gauges.

### Battery Cases

4x AA battery boxes with PP3-style clips were used to store the flight 
batteries. Two of these were installed in the nosecone and one on the body tube 
mounting plate.

### Arming Switches

Keyed arming switches were used to control power to the avionics, with one in 
the nosecone and one on the body tube.

### Status LEDs

Bicolour status LEDs were used to indicate flight computer status. At launch, 
only one was fitted, on the body tube, connected to the body tube `M2FC`.

### Antenna

For the main radio downlink, a 434MHz folded bazooka dipole antenna was fitted 
in the upper section of the nosecone, secured to a nylon rod which ran through 
the nosecone avionics mounting.

## Interconnections

In the body of Martlet 2 is one `M2FC` connected to: the fin strain gauges and 
thermocouples, the body `M2RL`, the Metron to release the main parachute, a 
single battery box, an arming switch, a status LED, and providing power via the 
auxiliary connector to one camera (and `M2RL`). These devices are all mounted 
on the body tube bulkhead plate.

Most of the avionics are in the nosecone, where `M2FC` is connected to `M2R`, 
the nosecone `M2RL`, three Metrons for separating the nosecone and two battery 
boxes in parallel (each 4xAA Lithium primary cells). Its auxiliary power 
connector provides power to two cameras, `M2RL` and `M2R`. `M2R` is connected 
to the RockBLOCK and `M2PA` which in turn is connected to the antenna. These 
devices were all mounted on an aluminium structure affixed to the base of the 
nosecone.

See the wiring diagram for a more detailed representation.

![System Overview](m2-wiring.pdf)

# Project Management

## Time Management

## Collaboration

## Prioritisation and Feature Freeze

# System Review

Little thought was put into the whole system design early on, such as 
interlinking the avionics and regulating and distributing the power supply. 
Much of this had to be designed once the rest of the avionics were finalised, 
which led to a suboptimal solution.

\begin{shaded}Consider how individual components will form the complete system 
before finalising their design.\end{shaded}

# Hardware Review

This section will address the design, manufacture, and testing of the avionics.

## Technology Choices

All PCB design was performed in KiCAD. While a few issues were encountered due 
to version upgrades during the development programme, in general this software 
worked well.

Setting up a common library of component symbols and footprints from the 
beginning would probably have saved time over copying symbols separately for 
each each project.

\begin{shaded}Establish a common library for symbols and footprints which all 
projects can share.\end{shaded}

## Design Procedure

The current procedure for carefully checking schematic and board layout led to 
only a single mistake in production: the powered isolator `ADuM5201` was given 
a normal-width `SOIC16` footprint where the datasheet called for a wide 
variant. Luckily the chip was easily modified be folding the legs around so 
this did not present a serious issue in production.

\begin{shaded}Careful and thorough checking of the PCB designs, including 
against the CUSF checklist, resulted in very few issues on a complicated 
design.\end{shaded}

Manufacture was by reflow soldering and had no issues beyond a few small solder 
bridges which were readily reworked. Again standard procedures seem to have 
worked very well here.


## M2FC

The early design called for a pyrotechnic board `M2P` to control the recovery 
system and a datalogging board `M2D` to record flight data. The decision was 
made early on to combine the pyrotechnic board and the datalogging board into 
the flight computer `M2FC`, as they required some very similar sensors and 
software. In retrospect, it seems likely that keeping these separate would have 
been beneficial: the recovery system could be simpler and thus more reliable, 
and the datalogger could be worked in isolation allowing better development 
collaboration with less risk of compromising the main objectives.

\begin{shaded}
Consider maintaining modularity even when integration is tempting; it preserves 
isolation and separation of concerns.
\end{shaded}

### Analogue Section

`M2FC`'s analogue section is responsible for converting the small electrical 
signals from the thermocouples and strain gauges into useful levels at the ADC. 
The design used specialised amplifiers as a frontend followed by a dual active 
antialiasing filter. These parts ended up being a substantial fraction of the 
total avionics cost, but were high specification and performed as required 
during testing and during flight. Additional testing and characterisation would 
have been useful and more time could have been allocated to this. In future 
less potent antialiasing filters might be sufficient but this design appears to 
have worked well.

### Pyrotechnic Drivers

The pyrotechnic channels consisted of a small N-channel MOSFET driving the gate 
of a higher current P-channel MOSFET which controlled high side power to the 
Metron protractors. Each channel additionally had a high power 5$\Omega$ 
resistor in series to limit total current, and a continuity measurement 
circuit. The high power resistors are physically large devices and it unclear 
how crucial they are; additional testing or characterisation may be useful in 
future.

The channels were not tested with the actual Metron devices until it was far 
too late to remedy any defects, instead only being tested with other 
pyrotechnic devices. This was a serious testing defect. In the event, the DC 
current provided through the pyrotechnic channel was insufficient to fire a 
Metron, but luckily using 10ms duration pulses instead proved reliable. The 
datasheet for the Metron devices claims a lower all-fire current with such 
pulses, so this is to be expected. This lucky escape meant no hardware 
alterations had to be performed.

\begin{shaded}Test pyrotechnic drivers with the flight pyrotechnic 
devices.\end{shaded}

The continuity testing worked as designed. In future resistance measurement 
would provide more information and may be able to indicate faulty connections.

### Inertial Measurement Unit

The IMU consisted of a number of low-cost individual MEMS sensors. On the whole 
this design worked to within acceptable limits, with some issues.

The two accelerometers worked well and having the two acceleration ranges 
covered was helpful. The barometer worked very well but did not appear to 
indicate when a conversion was complete despite datasheet claims, so had to be 
polled instead, though this only caused a slightly lower data rate.

The gyroscope appeared somewhat unreliable and additionally is a more obtuse 
sensor than the accelerometer. Some fault likely lies in the additional 
complexity of I2C instead of SPI. In future attempting to stick to SPI devices 
would probably simplify matters.

The magnetometer had similar issues to the gyroscope. Neither were required for 
flight computer operation however, so this did not present issues in this 
flight. For flights requiring a higher degree of state estimation, it may be 
prudent to select alternative gyroscopes and magnetometers.

Fully and partially integrated IMU sensors are available which would reduce 
alignment errors and simplify communication and drivers.

\begin{shaded}Consider using a more integrated IMU solution, or at least keep 
everything on an SPI bus and choose a different magnetometer and 
gyroscope.\end{shaded}

### SD Card

Both `M2FC`s had an SD card to store flight logs. SD cards have the serious 
advantages of high storage density, high write speeds, low cost, ready 
availability, high survivability, and ease of connection to a computer. 
Interfacing to them was straightforward and allowed high rate data to be stored 
without concern for filling up the storage space.

Mechanically they presented some issues. Connectors tend to emphasise ease of 
insertion/removal which is undesirable in this application. There were concerns 
that high vibrations and accelerations would cause the SD cards to bounce on 
their contact pins or even eject from the holder. The only obvious remedy was 
to use adhesive to secure the SD card, which complicates reading data off them 
after the flight.

Alternatives were considered such as onboard flash memory or eMMC. Onboard 
flash can be soldered down, but tends to either come in very low storage space, 
or large packages with high pin count parallel interfaces. eMMC is even worse 
in this regard, with most packages difficult to solder and requiring more 
complicated interface circuitry.

In the event some issues were encountered with SD card retention; see below for 
details.

On balance the SD card seems the best solution for high rate data, but perhaps 
lower rate flash storage could be useful as a backup for critical data.

### Off-Board Integrations

## M2R

### Radio

### GPS

### RockBLOCK

### Buzzer

## M2RL

The original design to interconnect the two `M2FC` boards, in the nosecone and 
in the body tube, was to use pogo pins on an interface board mounted near the 
nosecone separation point. This design proved very fiddly and delicate, with 
the pogo pins repeatedly breaking off during assembly. A more robust connector 
would be required for this sort of interface.

After the 2014 scrub, `M2RL` was designed to provide a radio link between the 
boards instead. This was much easier to integrate with the rocket design. 
Unfortunately time constraints meant this was not active during the flight, so 
its potential performance is unknown.

In future such radio links could be integrated onto the flight computers or 
possibly just downlinked to the ground station directly.

## M2PA

Originally a power amplifier based on the `ADL5324` device was installed on 
`M2R` but being integrated made it difficult to test and tune. After the 
scrubbed launch the amplifier was moved to its own board, `M2PA`. This made 
testing easier but took substantially more payload space.

The amplifier design was fairly successful, meeting specification. In future 
this design could probably be used integrated. Alternative amplifiers have 
become available which require substantially fewer external tuning components 
and offer higher powers which might be superior, such as the Skyworks 
`SKY65116`.

After the 2014 scrub, the power amplifier was moved off `M2R` and into its own 
box. This design worked well and the amplifier survived the flight. It's 
possible that in future returning to an integrated amplifier once the design is 
proven would have worked well too, simplifying avionics wiring.


## JOEY

As a backup locating device, a Joey tracker was installed in Martlet 2 as well. 
It had its own independent power supply, GPS, and radio. While this was not 
required in the event, its presence was reassuring and there is no reason to 
believe it would not have worked (had it not been torn to pieces during 
flight).

\begin{shaded}Use a totally separate and independent tracker for 
backup.\end{shaded}


## Arming Concept

The design for arming the avionics system was essentially a power switch on the 
body of the rocket, which controlled all power to the avionics. Obviously with 
the switch off, no pyrotechnic devices could fire, ensuring the rocket was 
safe. Two power switches were required, on the nosecone and on the body tube.

The connection to the power switch was provided on `M2FC`, which connected 
directly to the battery pack and then placed the arming switch in series before 
drawing any power.

This design was fairly simple but several issues were encountered

## Power Management

The power distribution design was not considered until after the majority of 
the avionics were finalised. Each piece of avionics had been designed with the 
assumption that somehow around 6-7V of battery power would be available, as an 
appropriate voltage to regulate down to 5V and 3.3V for electronics systems 
while also providing enough margin to reliably fire the Metron protractors used 
in the recovery system. The design was therefore to use 4xAA lithium primary 
cells in a battery, connected to `M2FC`, which would then power the rest of the 
system once armed through the `AUX` connector.

In the original design, `M2R` had its own separate battery supply and thus 
arming/power switch. In the 2014 scrub update, `M2R` is instead powered on the 
`AUX` bus from `M2FC`, which uses both battery packs in parallel to give 
maximum capacity for the combined system. This new design allowed for a single 
power switch which reduced complexity.

\begin{shaded}Don't use separate batteries for otherwise interdependent and 
connected systems, it just increases complexity. Central power switching is 
nice.\end{shaded}

Using lithium primary cells had some advantages for mass, power density, 
availability, and transport. CU Spaceflight have successfully used these cells 
for many varied missions so they were an obvious choice here. Being able to 
install a known-fresh set of batteries immediately prior to flight was 
reassuring, and being able to purchase these batteries at most large shops 
worldwide was likewise useful.

However, they necessitate battery holders which were all either difficult to 
mount securely, awkwardly shaped, or both. Reliable connections to the cells 
under the high acceleration conditions expected in flight was a pressing 
concern. In the original design, large Bulgin holders were used, but they were 
very awkward to fit in the mounting design. Post-scrub, smaller regular holders 
were used, but they did not provide any mounting points and were essentially 
friction fitted into a smaller compartment. Neither design was ideal, although 
it appears that the eventual design did provide power during acceleration.

In future it would be well worth considering a lithium ion rechargable main 
system battery with a dedicated power distribution board that can monitor and 
provide power to system components individually. Voltages could be regulated to 
some extent centrally, and supply currents could be measured, providing useful 
debugging information. Recharging allows for shore power to be used via an 
umbilical, which means the rocket may be powered up and on the pad for long 
durations, reducing urgency once padded up.

\begin{shaded}Give more thought to power supply design and distribution, 
including the option of rechargable batteries and an umbilical providing shore 
power.\end{shaded}

## Other Electronics

Beyond `M2FC` and `M2R` there were several other electronics systems on Martlet 
2, some integrated with the main avionics.

### RockBLOCK

`M2R` connected to a RockBLOCK Iridium satellite modem, which could relay 
position information from anywhere in the world. While a nice backup, this was 
probably not necessary at the launch site (a complete flat) and added 
complication to the avionics design and mounting. In the event the satellite 
modem did not have a chance to send any messages. In theory it is quite 
functional and would be useful on a future project where this level of backup 
communication is required.

\begin{shaded}Satellite modems can work well but consider if it is really 
required for a given flight, compared to the mass, power, and complexity 
cost.\end{shaded}

### PerfectFlite

To ensure the altitude reached would be recognised for record keeping purposes, 
a commercial altimeter (a PerfectFlite model) was fitted. No issues were found 
with this totally isolated system, although again clearer design on mounting 
and integration would have simplified launch operations.

### Cameras

We originally fitted a GoPro Hero camera and a Mobius ActionCam, both activated 
manually (the GoPro over the radio) before launch. After the 2014 scrub, the 
camera plan was updated to use only "808 #16" cameras, configured to begin 
recording when power is applied. They were then connected to `M2FC`'s auxiliary 
power output, so they began recording when the rocket was armed. This system 
worked very well and made final arming easier. Unfortunately during the flight 
anomaly the cameras were broken up and the SD cards were lost.

\begin{shaded}Small 808 cameras worked well. Activating them via the main 
avionics was a good idea. In future, secure the SD cards as well as possible, 
perhaps by gluing.\end{shaded}

## Other Components

### Metrons

### Strain Gauges

* Assembly issues
* Potential to short out excitation, putting whole flight computer at risk

### Thermocouples

### Battery Cases

### Arming Switches

### Status LEDs

### Antenna

## Interconnects

* Spec55 is wonderful
* JST PA did a good job
* External connectors in future
* Connector in the airframe?
* Umbilical to shore?

## Manufacture

### Issues Encountered

## Testing

# Software Review

## Technology Choices

## Reliability Management

### Watchdogs

### Hardware Error Handling

## Calibration

* Accels calibrate offset obviously, need to rotate for scale

## Drivers

## Flight State Machine

### Safety Timeout

### Ignition Detection

## Status Collection and Reporting

## Telemetry Collection, Forwarding and Storage

## Software Interfaces

## Testing

## Ground Station Software

## Release Management

# Integration Review

## Mounting

* Don't glue things
    * Really Really Really
    * LED broke off
    * Arming switch broke off
    * Other arming switch broke off
    * Arming key broke off
    * Both cameras broke off eventually
    * Don't glue things

## Enclosures

* More enclosed
* Easier to assemble
* No need to open to reprogram etc

## Switches and LEDs

## Cameras

## Antennas

## Cabling

## Sensors

# Launch Retrospective

## Missing Features

## Assembly

## Performance

### Radio

### Datalogging

### Sensors

### State Estimation

### Pyrotechnics

### Flight State

## Issues Encountered

### No RockBLOCK Signal In Enclosure

### Accidental Metron Firing During Assembly

### Arming Switch Location

* Too high up
* Behind launch rail

### Radio Not Enabled

* Required reprogramming

### Arming Key Broke

### Arming Switch Detached

### Status LED Detachedo

### Upright Requirement

### Ignition Detection Sensitivity

### Lack of Safety Timeout

### Early Body Metron Firing

### SD Cards Lost

# Conclusions and Recommendations


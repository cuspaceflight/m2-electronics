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

# Introduction

Martlet 2 was an amateur rocket designed, built, and launched by Cambridge 
University Spaceflight in 2013-2015. It was intended to reach a speed of around 
1km/s and an altitude of around 15km, establishing a new UK amateur altitude 
record.

After a scrubbed launch due to weather in October 2014, Martlet 2 was launched 
in October 2015 from Black Rock Desert, Nevada, USA. An anomaly occurred three 
seconds into the flight, resulting in the rapid and thorough destruction of the 
rocket including the avionics.

![Martlet 2 in Black Rock Desert, Nevada, in October 2014](m2_2014.jpg)

A custom set of avionics was designed for Martlet 2. After the scrubbed launch, 
some were updated to address issues in the original designs. This document is a 
postmortem and a retrospective of those avionics, reviewing the original design 
and evaluating the choices made and lessons learnt for future avionics 
programmes.

This postmortem is concerned with the avionics and their integration with the 
larger rocket system, but will not discuss matters solely related to the rocket 
or other components, nor will it detail the flight anomaly except insofar as it 
relates to the avionics design and possibility for future improvements.

\newpage

## Objectives

The original objectives for the avionics were, in priority order:

1. Control the recovery system deployment
1. Downlink rocket position information for recovery purposes
1. Record vital rocket parameters (altitude, velocity, acceleration, location)
1. Downlink additional information
1. Record additional sensors, including inertial, strain, and temperature

\begin{shaded}
Think carefully about precise objectives and priority. Lots of time was wasted 
on things that were not mission critical to the detriment of those that were.
\end{shaded}

# Hardware

## Hardware Overview

This section presents the hardware as-designed to give context to the rest of 
the analysis.

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

There are two `M2FC` boards on Martlet 2: one in the nosecone with the main 
avionics stack, responsible for firing the Metron protractors which separate 
the nosecone from the body of the rocket and for communicating with `M2R`; 
another in the body of the rocket responsible for firing the Metron protractor 
which releases the main parachute and for recording the strain gauges and 
thermocouples in the rocket fins.

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

### Other Hardware

Additionally there are two small cameras, a RockBLOCK satellite modem, two 
arming switches, three battery boxes, and four Metron protractors.

Installed in the fin are three strain gauges in a half-bridge configuration, 
with an unloaded set of strain gauges to provide temperature compensation, and 
a thermocouple to measure the fin root temperature.

### Interconnections

In the body of Martlet 2 is one `M2FC` connected to: the fin strain gauges and 
thermocouples, the body `M2RL`, the Metron to release the main parachute, a 
single battery box, an arming switch, a status LED, and providing power via the 
auxiliary connector to one camera (and `M2RL`).

Most of the avionics are in the nosecone, where `M2FC` is connected to `M2R`, 
the nosecone `M2RL`, three Metrons for separating the nosecone and two battery 
boxes in parallel (each 4xAA Lithium primary cells). Its auxiliary power 
connector provides power to two cameras, `M2RL` and `M2R`.

See the wiring diagram for a more detailed representation.

![System Overview](m2-wiring.pdf)

Little thought was put into the whole system design early on, such as 
interlinking the avionics and regulating and distributing the power supply. 
Much of this had to be designed once the rest of the avionics were finalised, 
which led to a worse solution.

\begin{shaded}Consider how individual components will form the complete system 
before finalising their design.\end{shaded}

## PCB Design & Manufacture

All PCB design was performed in KiCAD. Setting up a common library of component 
symbols and footprints from the beginning would probably have saved time.
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
circuit.

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

The continuity testing worked as designed.

### Inertial Measurement Unit

The IMU consisted of a number of low-cost individual MEMS sensors. On the whole 
this design worked to within acceptable limits, with some issues.

The two accelerometers worked well and having the two acceleration ranges 
covered was helpful. The barometer worked very well but did not appear to 
indicate when a conversion was complete despite datasheet claims, so had to be 
polled instead, though this only caused a slightly lower data rate.

The gyroscope appeared somewhat unreliable and additionally is a more obtuse 
sensor than the accelerometer. Some fault likely lies in the lower reliability 
of I2C over SPI. In future attempting to stick to SPI devices only would 
probably simplify matters.

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

* Came off. Solder/attach better next time.
* eMMC or onboard flash sounds nice but seems a lot less practical

### Radio Power Amplifier

### Inter-Board Communications

* M2RL a bit weird, could have been designed into the boards. Or don't have 
  that sort of communication and just have everything link to the ground. Might 
  be useful for multi-stage rockets. Unsure.

### Arming Concept

## Other Hardware

Beyond `M2FC` and `M2R` there were several other electronics systems on Martlet 
2, some integrated with the main avionics.

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

### Backup Tracker

As a backup locating device, a Joey tracker was installed in Martlet 2 as well. 
It had its own independent power supply, GPS, and radio. While this was not 
required in the event, its presence was reassuring and there is no reason to 
believe it would not have worked (had it not been torn to pieces during 
flight).

\begin{shaded}Use a totally separate and independent tracker for 
backup.\end{shaded}

### Recognised Altimeter

To ensure the altitude reached would be recognised for record keeping purposes, 
a commercial altimeter (a PerfectFlite model) was fitted. This was necessary 
but otherwise did not add a huge amount to the system. No particular issues 
were encountered here.

### Satellite Modem

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

### Radio Power Amplifier

After the 2014 scrub, the power amplifier was moved off `M2R` and into its own 
box. This design worked well and the amplifier survived the flight. It's 
possible that in future returning to an integrated amplifier once the design is 
proven would have worked well too, simplifying avionics wiring.

### `M2RL`

### Strain Gauges and Thermocouples

* Assembly issues
* Potential to short out excitation, putting whole flight computer at risk

### Power Supply & Distribution

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

### Cabling & Connectors

* Spec55 is wonderful
* JST PA did a good job
* External connectors in future
* Connector in the airframe?
* Umbilical to shore?

## Mounting & Enclosure

* More enclosed
* Easier to assemble
* No need to open to reprogram etc

## External Switches & Arming

## Human Interface

## Testing

# Software

## Design

### Real-Time Operating System

### Calibration
* Accels calibrate offset obviously, need to rotate for scale

### Powerup Sequence

### Safety Lockout

### Ignition Detection

### Status Monitoring & Sharing

## Implementation

### Timing

### Testing

## Ground Software

* Would have been nice...

## Collaboration

# Integration

* Mounting holes
* LEDs
* Sensors (SG/TC)
* Antennas
* Cameras
* Don't glue things
    * Really Really Really
    * LED broke off
    * Arming switch broke off
    * Other arming switch broke off
    * Arming key broke off
    * Both cameras broke off eventually
    * Don't glue things

# Recommendations

# Conclusions


# M2 Electronics Postmortem Documentation: Planning Notes

## Structure

1. Introduction, Scope, Objectives
1. Hardware Design Review
1. Hardware Manufacture Review
1. Firmware Design Review
1. Firmware Implementation Review
1. Performance Analysis
1. Bugs and Other Issues
1. Lessons Learnt & Recommendations
1. Conclusions

## Structure Take 2

1. Summary
    1. Brief overview
    1. Key successes
    1. Key problems
    1. Key takeaways
1. Introduction
    1. Martlet 2 Overview
    1. Report scope and goals
    1. Project objectives
1. Avionics Overview
    1. Custom Hardware
        1. M2FC
        1. M2R
        1. M2RL
        1. M2PA
        1. JOEY Tracker
    1. Other Electronics
        1. RockBLOCK
        1. PerfectFlite
        1. Cameras
    1. Other Components
        1. Metrons
        1. Strain gauges
        1. Thermocouples
        1. Battery cases
        1. Arming switches
        1. Status LEDs
        1. Antennas
    1. Interconnection
1. Project Management
    1. Time management
    1. Collaboration
    1. Prioritisaton and Feature Freeze
1. System Review
1. Hardware Review
    1. Technology Choices (KiCAD)
    1. Design Procedure
    1. M2FC
        1. Analogue
        1. Pyrotechnics
        1. IMU
        1. SD Card
        1. Off-board integrations
    1. M2R
        1. Radio
        1. GPS
        1. RockBLOCK
        1. Buzzer
    1. M2RL
    1. M2PA
    1. Arming System
    1. Power System
    1. Other Electronics
        1. RockBLOCK
        1. PerfectFlite
        1. Cameras
    1. Other Components
        1. Metrons
        1. Strain gauges
        1. Thermocouples
        1. Battery cases
        1. Arming switches
        1. Status LEDs
        1. Antennas
    1. Interconnections
        1. JST PA
        1. Spec 55
        1. Radio links
    1. Manufacture
        1. Issues Encountered
    1. Testing
1. Software Review
    1. Technology Choices (C, git, ChibiOS)
    1. Reliability Management
        1. Watchdogs
        1. Hardware error handling
    1. Drivers
    1. Calibration
    1. Flight state machine
        1. Safety Timeouts
        1. Ignition Detection
    1. Status reporting and collection
    1. Telemetry collection and forwarding/storage
    1. Software interfaces
    1. Testing
    1. Ground station software
    1. Revision Control / Release Process
1. Integration Review
    1. Mounting
    1. Enclosures
    1. Switches and LEDs
    1. Cameras
    1. Antennas
    1. Cabling
    1. Sensors
1. Launch Retrospective
    1. Missing Features
        1. IMU Magno/Gyro
        1. ADC recording
        1. M2RL
    1. Asssembly
    1. Performance
        1. Radio
        1. Datalogging
        1. Sensors
        1. State estimation
    1. Issues Encountered
        1. No RockBLOCK signal while in enclosure
        1. Accidental body Metron firing in assembly
        1. Arming switch high up
        1. Arming switch behind launch rail
        1. Radio not enabled, requiring reprogramming
        1. Key breaking apart
        1. Keyswitch breaking off
        1. LED breaking off
        1. Upright on arming requirement
        1. No safety timeout
        1. Sensitive ignition detection
        1. Early body Metron firing while on pad
        1. SD cards lost
1. Conclusions & Recommendations

    

## Thoughts To Include
Per section: Overview, Performance, Issues, Lessons Learnt

## Links

GitHub repo: https://github.com/cuspaceflight/m2-electronics
Photos: https://www.flickr.com/photos/randomskk/albums/72157645881007740

## Thoughts drop

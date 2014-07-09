EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:adum5201
LIBS:adxl375
LIBS:apu1205-hf-3
LIBS:m2fc-cache
LIBS:part
LIBS:sp100-2t
LIBS:stm32f303c
LIBS:swd
LIBS:testpad
LIBS:adxl345
LIBS:l3g4200d
LIBS:hmc5883l
LIBS:ad8495
LIBS:ad8226
LIBS:tmp36
LIBS:ms5611-01ba03
LIBS:op1177
LIBS:stm32f415rg
LIBS:op2177
LIBS:microsd
LIBS:adp3335
LIBS:ad8541
LIBS:ad8656
LIBS:stm32f405vgt
LIBS:m2fc-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 6
Title "Martlet 2 Flight Computer"
Date "05 Jul 2014"
Rev "1"
Comp "Cambridge University Spaceflight"
Comment1 "Drawn By: Adam Greig"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_4 P?
U 1 1 53B6A62B
P 6150 3250
F 0 "P?" V 6100 3250 50  0000 C CNN
F 1 "CONN_4" V 6200 3250 50  0000 C CNN
F 2 "" H 6150 3250 60  0001 C CNN
F 3 "" H 6150 3250 60  0001 C CNN
F 4 "1462920" V 6150 3250 60  0001 C CNN "Farnell"
	1    6150 3250
	1    0    0    -1  
$EndComp
$Comp
L ADuM5201 IC?
U 1 1 53B6A633
P 4700 3300
F 0 "IC?" H 4500 3700 60  0000 C CNN
F 1 "ADuM5201" H 4650 2800 60  0000 C CNN
F 2 "" H 4800 3300 60  0000 C CNN
F 3 "" H 4800 3300 60  0000 C CNN
F 4 "2067773" H 4700 3300 60  0001 C CNN "Farnell"
	1    4700 3300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 53B6A63B
P 4000 2850
F 0 "C?" H 4050 2950 50  0000 L CNN
F 1 "100n" H 4050 2750 50  0000 L CNN
F 2 "0603" H 4000 2850 60  0001 C CNN
F 3 "" H 4000 2850 60  0001 C CNN
F 4 "1759037" H 4000 2850 60  0001 C CNN "Farnell"
	1    4000 2850
	0    -1   -1   0   
$EndComp
$Comp
L CP1 C?
U 1 1 53B6A643
P 4000 2500
F 0 "C?" H 4050 2600 50  0000 L CNN
F 1 "10µ" H 4050 2400 50  0000 L CNN
F 2 "1411" H 4000 2500 60  0001 C CNN
F 3 "" H 4000 2500 60  0001 C CNN
F 4 "2333026" H 4000 2500 60  0001 C CNN "Farnell"
	1    4000 2500
	0    1    -1   0   
$EndComp
$Comp
L C C?
U 1 1 53B6A64B
P 5450 2850
F 0 "C?" H 5500 2950 50  0000 L CNN
F 1 "100n" H 5500 2750 50  0000 L CNN
F 2 "0603" H 5450 2850 60  0001 C CNN
F 3 "" H 5450 2850 60  0001 C CNN
F 4 "1759037" H 5450 2850 60  0001 C CNN "Farnell"
	1    5450 2850
	0    -1   -1   0   
$EndComp
$Comp
L CP1 C?
U 1 1 53B6A653
P 5450 2600
F 0 "C?" H 5500 2700 50  0000 L CNN
F 1 "10µ" H 5500 2500 50  0000 L CNN
F 2 "1411" H 5450 2600 60  0001 C CNN
F 3 "" H 5450 2600 60  0001 C CNN
F 4 "2333026" H 5450 2600 60  0001 C CNN "Farnell"
	1    5450 2600
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR078
U 1 1 53B6A65A
P 4200 3400
F 0 "#PWR078" H 4200 3400 30  0001 C CNN
F 1 "GND" H 4200 3330 30  0001 C CNN
F 2 "" H 4200 3400 60  0001 C CNN
F 3 "" H 4200 3400 60  0001 C CNN
	1    4200 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR079
U 1 1 53B6A660
P 4250 3750
F 0 "#PWR079" H 4250 3750 30  0001 C CNN
F 1 "GND" H 4250 3680 30  0001 C CNN
F 2 "" H 4250 3750 60  0001 C CNN
F 3 "" H 4250 3750 60  0001 C CNN
	1    4250 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR080
U 1 1 53B6A668
P 3750 3000
F 0 "#PWR080" H 3750 3000 30  0001 C CNN
F 1 "GND" H 3750 2930 30  0001 C CNN
F 2 "" H 3750 3000 60  0001 C CNN
F 3 "" H 3750 3000 60  0001 C CNN
	1    3750 3000
	1    0    0    -1  
$EndComp
$Comp
L CONN_4 P?
U 1 1 53B6A66F
P 6150 4750
F 0 "P?" V 6100 4750 50  0000 C CNN
F 1 "CONN_4" V 6200 4750 50  0000 C CNN
F 2 "" H 6150 4750 60  0001 C CNN
F 3 "" H 6150 4750 60  0001 C CNN
F 4 "1462920" V 6150 4750 60  0001 C CNN "Farnell"
	1    6150 4750
	1    0    0    -1  
$EndComp
$Comp
L ADuM5201 IC?
U 1 1 53B6A677
P 4700 4800
F 0 "IC?" H 4500 5200 60  0000 C CNN
F 1 "ADuM5201" H 4650 4300 60  0000 C CNN
F 2 "" H 4800 4800 60  0000 C CNN
F 3 "" H 4800 4800 60  0000 C CNN
F 4 "2067773" H 4700 4800 60  0001 C CNN "Farnell"
	1    4700 4800
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 53B6A67F
P 4000 4350
F 0 "C?" H 4050 4450 50  0000 L CNN
F 1 "100n" H 4050 4250 50  0000 L CNN
F 2 "0603" H 4000 4350 60  0001 C CNN
F 3 "" H 4000 4350 60  0001 C CNN
F 4 "1759037" H 4000 4350 60  0001 C CNN "Farnell"
	1    4000 4350
	0    -1   -1   0   
$EndComp
$Comp
L CP1 C?
U 1 1 53B6A687
P 4000 4000
F 0 "C?" H 4050 4100 50  0000 L CNN
F 1 "10µ" H 4050 3900 50  0000 L CNN
F 2 "1411" H 4000 4000 60  0001 C CNN
F 3 "" H 4000 4000 60  0001 C CNN
F 4 "2333026" H 4000 4000 60  0001 C CNN "Farnell"
	1    4000 4000
	0    1    -1   0   
$EndComp
$Comp
L C C?
U 1 1 53B6A68F
P 5450 4350
F 0 "C?" H 5500 4450 50  0000 L CNN
F 1 "100n" H 5500 4250 50  0000 L CNN
F 2 "0603" H 5450 4350 60  0001 C CNN
F 3 "" H 5450 4350 60  0001 C CNN
F 4 "1759037" H 5450 4350 60  0001 C CNN "Farnell"
	1    5450 4350
	0    -1   -1   0   
$EndComp
$Comp
L CP1 C?
U 1 1 53B6A697
P 5450 4100
F 0 "C?" H 5500 4200 50  0000 L CNN
F 1 "10µ" H 5500 4000 50  0000 L CNN
F 2 "1411" H 5450 4100 60  0001 C CNN
F 3 "" H 5450 4100 60  0001 C CNN
F 4 "2333026" H 5450 4100 60  0001 C CNN "Farnell"
	1    5450 4100
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR081
U 1 1 53B6A69E
P 4200 4900
F 0 "#PWR081" H 4200 4900 30  0001 C CNN
F 1 "GND" H 4200 4830 30  0001 C CNN
F 2 "" H 4200 4900 60  0001 C CNN
F 3 "" H 4200 4900 60  0001 C CNN
	1    4200 4900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR082
U 1 1 53B6A6A4
P 4250 5250
F 0 "#PWR082" H 4250 5250 30  0001 C CNN
F 1 "GND" H 4250 5180 30  0001 C CNN
F 2 "" H 4250 5250 60  0001 C CNN
F 3 "" H 4250 5250 60  0001 C CNN
	1    4250 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR083
U 1 1 53B6A6AC
P 3750 4500
F 0 "#PWR083" H 3750 4500 30  0001 C CNN
F 1 "GND" H 3750 4430 30  0001 C CNN
F 2 "" H 3750 4500 60  0001 C CNN
F 3 "" H 3750 4500 60  0001 C CNN
	1    3750 4500
	1    0    0    -1  
$EndComp
Text Notes 3350 5450 0    60   ~ 0
SERIAL IN/OUT
$Comp
L +3.3V #PWR084
U 1 1 53B6A6B3
P 4100 5000
F 0 "#PWR084" H 4100 4960 30  0001 C CNN
F 1 "+3.3V" H 4100 5110 30  0000 C CNN
F 2 "" H 4100 5000 60  0000 C CNN
F 3 "" H 4100 5000 60  0000 C CNN
	1    4100 5000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR085
U 1 1 53B6A6B9
P 4100 3500
F 0 "#PWR085" H 4100 3460 30  0001 C CNN
F 1 "+3.3V" H 4100 3610 30  0000 C CNN
F 2 "" H 4100 3500 60  0000 C CNN
F 3 "" H 4100 3500 60  0000 C CNN
	1    4100 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3400 4300 3350
Wire Wire Line
	4300 3350 4200 3350
Wire Wire Line
	4200 3350 4200 3400
Wire Wire Line
	4100 3500 4300 3500
Wire Wire Line
	4300 3600 4250 3600
Wire Wire Line
	4250 3600 4250 3500
Connection ~ 4250 3500
Wire Wire Line
	4300 3700 4250 3700
Wire Wire Line
	4250 3700 4250 3750
Wire Wire Line
	3950 3300 4300 3300
Wire Wire Line
	3950 3200 4300 3200
Wire Wire Line
	5150 3200 5800 3200
Wire Wire Line
	5150 3300 5800 3300
Wire Wire Line
	5150 3000 5800 3000
Wire Wire Line
	5200 2600 5200 3000
Wire Wire Line
	5200 2850 5250 2850
Wire Wire Line
	5200 2600 5250 2600
Connection ~ 5200 2850
Wire Wire Line
	5700 3100 5150 3100
Wire Wire Line
	5700 3700 5700 2600
Wire Wire Line
	5700 3400 5800 3400
Wire Wire Line
	5800 3000 5800 3100
Connection ~ 5200 3000
Wire Wire Line
	5650 2850 5700 2850
Connection ~ 5700 3100
Wire Wire Line
	5700 2600 5650 2600
Connection ~ 5700 2850
Wire Wire Line
	5150 3700 5700 3700
Connection ~ 5700 3400
Wire Wire Line
	5150 3500 5250 3500
Wire Wire Line
	5250 3500 5250 3700
Connection ~ 5250 3700
Wire Wire Line
	5150 3600 5350 3600
Wire Wire Line
	5350 3600 5350 3000
Connection ~ 5350 3000
Wire Wire Line
	4250 2500 4250 3000
Wire Wire Line
	4250 2850 4200 2850
Wire Wire Line
	4250 2500 4200 2500
Connection ~ 4250 2850
Wire Wire Line
	4250 3000 4300 3000
Wire Wire Line
	3800 2500 3750 2500
Wire Wire Line
	3800 2850 3750 2850
Connection ~ 3750 2850
Wire Wire Line
	3750 2500 3750 3000
Wire Wire Line
	4300 3100 3850 3100
Wire Wire Line
	3850 3100 3850 2950
Wire Wire Line
	3850 2950 3750 2950
Connection ~ 3750 2950
Wire Wire Line
	4300 4900 4300 4850
Wire Wire Line
	4300 4850 4200 4850
Wire Wire Line
	4200 4850 4200 4900
Wire Wire Line
	4100 5000 4300 5000
Wire Wire Line
	4300 5100 4250 5100
Wire Wire Line
	4250 5100 4250 5000
Connection ~ 4250 5000
Wire Wire Line
	4300 5200 4250 5200
Wire Wire Line
	4250 5200 4250 5250
Wire Wire Line
	3950 4800 4300 4800
Wire Wire Line
	3950 4700 4300 4700
Wire Wire Line
	5150 4700 5800 4700
Wire Wire Line
	5150 4800 5800 4800
Wire Wire Line
	5150 4500 5800 4500
Wire Wire Line
	5200 4100 5200 4500
Wire Wire Line
	5200 4350 5250 4350
Wire Wire Line
	5200 4100 5250 4100
Connection ~ 5200 4350
Wire Wire Line
	5700 4600 5150 4600
Wire Wire Line
	5700 5200 5700 4100
Wire Wire Line
	5700 4900 5800 4900
Wire Wire Line
	5800 4500 5800 4600
Connection ~ 5200 4500
Wire Wire Line
	5650 4350 5700 4350
Connection ~ 5700 4600
Wire Wire Line
	5700 4100 5650 4100
Connection ~ 5700 4350
Wire Wire Line
	5150 5200 5700 5200
Connection ~ 5700 4900
Wire Wire Line
	5150 5000 5250 5000
Wire Wire Line
	5250 5000 5250 5200
Connection ~ 5250 5200
Wire Wire Line
	5150 5100 5350 5100
Wire Wire Line
	5350 5100 5350 4500
Connection ~ 5350 4500
Wire Wire Line
	4250 4000 4250 4500
Wire Wire Line
	4250 4350 4200 4350
Wire Wire Line
	4250 4000 4200 4000
Connection ~ 4250 4350
Wire Wire Line
	4250 4500 4300 4500
Wire Wire Line
	3800 4000 3750 4000
Wire Wire Line
	3800 4350 3750 4350
Connection ~ 3750 4350
Wire Wire Line
	3750 4000 3750 4500
Wire Wire Line
	4300 4600 3850 4600
Wire Wire Line
	3850 4600 3850 4450
Wire Wire Line
	3850 4450 3750 4450
Connection ~ 3750 4450
Wire Notes Line
	6300 2300 6300 5500
Wire Notes Line
	3300 2300 3300 5500
Wire Notes Line
	3300 2300 6300 2300
Wire Notes Line
	3300 5500 6300 5500
Text Notes 6400 5450 0    60   ~ 0
EXTERNAL STATUS LED
$Comp
L PART U?
U 1 1 53B6AD6D
P 7700 4850
F 0 "U?" H 7750 4850 60  0000 C CNN
F 1 "RG LED" H 7850 4950 60  0000 C CNN
F 2 "" H 7700 4850 60  0001 C CNN
F 3 "" H 7700 4850 60  0001 C CNN
F 4 "1142488" H 7700 4850 60  0001 C CNN "Farnell"
	1    7700 4850
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P?
U 1 1 53B6AD75
P 8000 4500
F 0 "P?" V 7950 4500 40  0000 C CNN
F 1 "CONN_2" V 8050 4500 40  0000 C CNN
F 2 "" H 8000 4500 60  0000 C CNN
F 3 "" H 8000 4500 60  0000 C CNN
F 4 "1462926" H 8000 4500 60  0001 C CNN "Farnell"
	1    8000 4500
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 53B6AD98
P 7250 4450
F 0 "R?" V 7330 4450 50  0000 C CNN
F 1 "100" V 7250 4450 50  0000 C CNN
F 2 "0603" H 7250 4450 60  0001 C CNN
F 3 "" H 7250 4450 60  0001 C CNN
F 4 "2331989" V 7250 4450 60  0001 C CNN "Farnell"
	1    7250 4450
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 53B6ADA0
P 7250 4300
F 0 "R?" V 7330 4300 50  0000 C CNN
F 1 "100" V 7250 4300 50  0000 C CNN
F 2 "0603" H 7250 4300 60  0001 C CNN
F 3 "" H 7250 4300 60  0001 C CNN
F 4 "2331989" V 7250 4300 60  0001 C CNN "Farnell"
	1    7250 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 4600 7650 4600
Wire Wire Line
	7000 4400 6750 4400
Wire Wire Line
	7500 4400 7650 4400
Wire Notes Line
	6350 3550 8150 3550
Wire Notes Line
	8150 3550 8150 5500
Wire Notes Line
	8150 5500 6350 5500
Wire Notes Line
	6350 5500 6350 3550
Wire Wire Line
	7000 4300 7000 4450
Connection ~ 7000 4400
Wire Wire Line
	7500 4300 7500 4450
Connection ~ 7500 4400
$Comp
L LED D?
U 1 1 53B6B184
P 7350 2500
F 0 "D?" H 7350 2600 50  0000 C CNN
F 1 "RED" H 7350 2400 50  0000 C CNN
F 2 "0603" H 7350 2500 60  0001 C CNN
F 3 "" H 7350 2500 60  0000 C CNN
F 4 "2290329" H 7350 2500 60  0001 C CNN "Farnell"
	1    7350 2500
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 53B6B18C
P 6850 2500
F 0 "R?" V 6930 2500 50  0000 C CNN
F 1 "100" V 6850 2500 50  0000 C CNN
F 2 "0603" H 6850 2500 60  0001 C CNN
F 3 "" H 6850 2500 60  0001 C CNN
F 4 "2331989" V 6850 2500 60  0001 C CNN "Farnell"
	1    6850 2500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR086
U 1 1 53B6B193
P 7600 2550
F 0 "#PWR086" H 7600 2550 30  0001 C CNN
F 1 "GND" H 7600 2480 30  0001 C CNN
F 2 "" H 7600 2550 60  0001 C CNN
F 3 "" H 7600 2550 60  0001 C CNN
	1    7600 2550
	1    0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 53B6B19B
P 7700 2750
F 0 "D?" H 7700 2850 50  0000 C CNN
F 1 "YELLOW" H 7700 2650 50  0000 C CNN
F 2 "0603" H 7700 2750 60  0001 C CNN
F 3 "" H 7700 2750 60  0000 C CNN
F 4 "2290330" H 7700 2750 60  0001 C CNN "Farnell"
	1    7700 2750
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 53B6B1A3
P 7200 2750
F 0 "R?" V 7300 2750 50  0000 C CNN
F 1 "100" V 7200 2750 50  0000 C CNN
F 2 "0603" H 7200 2750 60  0001 C CNN
F 3 "" H 7200 2750 60  0001 C CNN
F 4 "2331989" V 7200 2750 60  0001 C CNN "Farnell"
	1    7200 2750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR087
U 1 1 53B6B1AA
P 7950 2800
F 0 "#PWR087" H 7950 2800 30  0001 C CNN
F 1 "GND" H 7950 2730 30  0001 C CNN
F 2 "" H 7950 2800 60  0001 C CNN
F 3 "" H 7950 2800 60  0001 C CNN
	1    7950 2800
	1    0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 53B6B1B2
P 7700 3000
F 0 "D?" H 7700 3100 50  0000 C CNN
F 1 "YELLOW" H 7700 2900 50  0000 C CNN
F 2 "0603" H 7700 3000 60  0001 C CNN
F 3 "" H 7700 3000 60  0000 C CNN
F 4 "2290330" H 7700 3000 60  0001 C CNN "Farnell"
	1    7700 3000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 53B6B1BA
P 7200 3000
F 0 "R?" V 7300 3000 50  0000 C CNN
F 1 "100" V 7200 3000 50  0000 C CNN
F 2 "0603" H 7200 3000 60  0001 C CNN
F 3 "" H 7200 3000 60  0001 C CNN
F 4 "2331989" V 7200 3000 60  0001 C CNN "Farnell"
	1    7200 3000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR088
U 1 1 53B6B1C1
P 7950 3050
F 0 "#PWR088" H 7950 3050 30  0001 C CNN
F 1 "GND" H 7950 2980 30  0001 C CNN
F 2 "" H 7950 3050 60  0001 C CNN
F 3 "" H 7950 3050 60  0001 C CNN
	1    7950 3050
	1    0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 53B6B1C9
P 7700 3250
F 0 "D?" H 7700 3350 50  0000 C CNN
F 1 "YELLOW" H 7700 3150 50  0000 C CNN
F 2 "0603" H 7700 3250 60  0001 C CNN
F 3 "" H 7700 3250 60  0000 C CNN
F 4 "2290330" H 7700 3250 60  0001 C CNN "Farnell"
	1    7700 3250
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 53B6B1D1
P 7200 3250
F 0 "R?" V 7300 3250 50  0000 C CNN
F 1 "100" V 7200 3250 50  0000 C CNN
F 2 "0603" H 7200 3250 60  0001 C CNN
F 3 "" H 7200 3250 60  0001 C CNN
F 4 "2331989" V 7200 3250 60  0001 C CNN "Farnell"
	1    7200 3250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR089
U 1 1 53B6B1D8
P 7950 3300
F 0 "#PWR089" H 7950 3300 30  0001 C CNN
F 1 "GND" H 7950 3230 30  0001 C CNN
F 2 "" H 7950 3300 60  0001 C CNN
F 3 "" H 7950 3300 60  0001 C CNN
	1    7950 3300
	1    0    0    -1  
$EndComp
Text Notes 6400 3450 0    60   ~ 0
INTERNAL STATUS LEDS
$Comp
L +3.3V #PWR090
U 1 1 53B6B1DF
P 6550 2450
F 0 "#PWR090" H 6550 2410 30  0001 C CNN
F 1 "+3.3V" H 6550 2560 30  0000 C CNN
F 2 "" H 6550 2450 60  0000 C CNN
F 3 "" H 6550 2450 60  0000 C CNN
	1    6550 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 2450 6550 2500
Wire Wire Line
	6550 2500 6600 2500
Wire Wire Line
	7100 2500 7150 2500
Wire Wire Line
	7550 2500 7600 2500
Wire Wire Line
	7600 2500 7600 2550
Wire Wire Line
	6900 2750 6950 2750
Wire Wire Line
	7450 2750 7500 2750
Wire Wire Line
	7900 2750 7950 2750
Wire Wire Line
	7950 2750 7950 2800
Wire Wire Line
	6900 3000 6950 3000
Wire Wire Line
	7450 3000 7500 3000
Wire Wire Line
	7900 3000 7950 3000
Wire Wire Line
	7950 3000 7950 3050
Wire Wire Line
	6900 3250 6950 3250
Wire Wire Line
	7450 3250 7500 3250
Wire Wire Line
	7900 3250 7950 3250
Wire Wire Line
	7950 3250 7950 3300
Wire Notes Line
	6350 2300 8150 2300
Wire Notes Line
	8150 2300 8150 3500
Wire Notes Line
	8150 3500 6350 3500
Wire Notes Line
	6350 3500 6350 2300
Text HLabel 3950 3200 0    60   Input ~ 0
SER_OUT_1
Text HLabel 3950 3300 0    60   Output ~ 0
SER_IN_1
Text HLabel 3950 4700 0    60   Input ~ 0
SER_OUT_2
Text HLabel 3950 4800 0    60   Output ~ 0
SER_IN_2
Text HLabel 6750 4400 0    60   Input ~ 0
LED_A
Text HLabel 6750 4600 0    60   Input ~ 0
LED_C
Text HLabel 6900 2750 0    60   Input ~ 0
LED_1
Text HLabel 6900 3000 0    60   Input ~ 0
LED_2
Text HLabel 6900 3250 0    60   Input ~ 0
LED_3
$Comp
L TESTPAD TP?
U 1 1 53B7DC62
P 4000 3200
F 0 "TP?" H 4030 3160 30  0000 C CNN
F 1 "TESTPAD" H 4000 3250 60  0001 C CNN
F 2 "" H 4000 3200 60  0000 C CNN
F 3 "" H 4000 3200 60  0000 C CNN
	1    4000 3200
	0    -1   -1   0   
$EndComp
$Comp
L TESTPAD TP?
U 1 1 53B7DD97
P 4000 3300
F 0 "TP?" H 4030 3260 30  0000 C CNN
F 1 "TESTPAD" H 4000 3350 60  0001 C CNN
F 2 "" H 4000 3300 60  0000 C CNN
F 3 "" H 4000 3300 60  0000 C CNN
	1    4000 3300
	0    -1   -1   0   
$EndComp
$Comp
L TESTPAD TP?
U 1 1 53B7DDC5
P 4000 4700
F 0 "TP?" H 4030 4660 30  0000 C CNN
F 1 "TESTPAD" H 4000 4750 60  0001 C CNN
F 2 "" H 4000 4700 60  0000 C CNN
F 3 "" H 4000 4700 60  0000 C CNN
	1    4000 4700
	0    -1   -1   0   
$EndComp
$Comp
L TESTPAD TP?
U 1 1 53B7DEA9
P 4000 4800
F 0 "TP?" H 4030 4760 30  0000 C CNN
F 1 "TESTPAD" H 4000 4850 60  0001 C CNN
F 2 "" H 4000 4800 60  0000 C CNN
F 3 "" H 4000 4800 60  0000 C CNN
	1    4000 4800
	0    -1   -1   0   
$EndComp
Connection ~ 4000 4800
Connection ~ 4000 4700
Connection ~ 4000 3200
Connection ~ 4000 3300
$Comp
L C C?
U 1 1 53BB2BD1
P 7400 4750
F 0 "C?" H 7450 4850 50  0000 L CNN
F 1 "100n" H 7450 4650 50  0000 L CNN
F 2 "0603" H 7400 4750 60  0001 C CNN
F 3 "" H 7400 4750 60  0001 C CNN
F 4 "1759037" H 7400 4750 60  0001 C CNN "Farnell"
	1    7400 4750
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 53BB30F0
P 7400 4050
F 0 "C?" H 7450 4150 50  0000 L CNN
F 1 "100n" H 7450 3950 50  0000 L CNN
F 2 "0603" H 7400 4050 60  0001 C CNN
F 3 "" H 7400 4050 60  0001 C CNN
F 4 "1759037" H 7400 4050 60  0001 C CNN "Farnell"
	1    7400 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 4600 7600 4750
Connection ~ 7600 4600
$Comp
L GND #PWR?
U 1 1 53BB3222
P 7150 4800
F 0 "#PWR?" H 7150 4800 30  0001 C CNN
F 1 "GND" H 7150 4730 30  0001 C CNN
F 2 "" H 7150 4800 60  0001 C CNN
F 3 "" H 7150 4800 60  0001 C CNN
	1    7150 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 4750 7150 4750
Wire Wire Line
	7150 4750 7150 4800
$Comp
L GND #PWR?
U 1 1 53BB32F5
P 7150 4100
F 0 "#PWR?" H 7150 4100 30  0001 C CNN
F 1 "GND" H 7150 4030 30  0001 C CNN
F 2 "" H 7150 4100 60  0001 C CNN
F 3 "" H 7150 4100 60  0001 C CNN
	1    7150 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 4050 7600 4400
Connection ~ 7600 4400
Wire Wire Line
	7200 4050 7150 4050
Wire Wire Line
	7150 4050 7150 4100
Text Notes 3350 2250 0    100  ~ 0
INPUT/OUTPUT
$EndSCHEMATC

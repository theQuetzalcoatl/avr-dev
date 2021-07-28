EESchema Schematic File Version 2
LIBS:panel-rescue
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
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
LIBS:MyLib
LIBS:panel-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 6
Title ""
Date "2020-07-18"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L R R18
U 1 1 5D11BC4D
P 2700 3200
F 0 "R18" V 2780 3200 50  0000 C CNN
F 1 "PT1000" H 2475 3275 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0411_L9.9mm_D3.6mm_P5.08mm_Vertical" V 2630 3200 50  0001 C CNN
F 3 "" H 2700 3200 50  0001 C CNN
	1    2700 3200
	1    0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 5D11BC61
P 2700 2300
F 0 "R17" V 2780 2300 50  0000 C CNN
F 1 "2k7" V 2700 2300 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2630 2300 50  0001 C CNN
F 3 "" H 2700 2300 50  0001 C CNN
	1    2700 2300
	1    0    0    -1  
$EndComp
$Comp
L C C17
U 1 1 5D11BC68
P 2975 3200
F 0 "C17" H 3000 3300 50  0000 L CNN
F 1 "10nF" H 3000 3100 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 3013 3050 50  0001 C CNN
F 3 "" H 2975 3200 50  0001 C CNN
	1    2975 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 2450 2700 2550
Wire Wire Line
	2700 2950 2700 3050
Wire Wire Line
	2100 2750 2400 2750
Wire Wire Line
	2100 3400 3275 3400
Wire Wire Line
	2700 3350 2700 3400
Wire Wire Line
	3275 3000 2700 3000
Connection ~ 2700 3000
Wire Wire Line
	2975 3000 2975 3050
Wire Wire Line
	2975 3350 2975 3400
Connection ~ 2700 3400
Connection ~ 2975 3000
Text HLabel 3275 3000 2    60   Input ~ 0
AD2
Text HLabel 2600 1525 0    60   Input ~ 0
5V
Wire Wire Line
	2700 1525 2600 1525
Text HLabel 3275 3400 2    60   Input ~ 0
GND
Connection ~ 2975 3400
Text Notes 3625 1875 0    60   ~ 0
Design notes:\n- load current will be approximately 1 mA\n- 0,4K/mW self-heating -> it shall be compensated by SW
$Comp
L BC807 Q2
U 1 1 5E80E972
P 2600 2750
F 0 "Q2" H 2800 2825 50  0000 L CNN
F 1 "BC807" H 2800 2750 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 2800 2675 50  0001 L CIN
F 3 "" H 2600 2750 50  0001 L CNN
	1    2600 2750
	1    0    0    1   
$EndComp
$Comp
L R R2
U 1 1 5FF64644
P 2700 1925
F 0 "R2" V 2780 1925 50  0000 C CNN
F 1 "200" V 2700 1925 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2630 1925 50  0001 C CNN
F 3 "" H 2700 1925 50  0001 C CNN
	1    2700 1925
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 2150 2700 2075
Wire Wire Line
	2700 1775 2700 1525
$Comp
L D_Zener_Small_ALT D1
U 1 1 5FF64A1F
P 2100 2150
F 0 "D1" H 2100 2240 50  0000 C CNN
F 1 "3_6BreakDown" V 2225 2175 50  0000 C CNN
F 2 "Diodes_SMD:D_MiniMELF_Handsoldering" V 2100 2150 50  0001 C CNN
F 3 "" V 2100 2150 50  0001 C CNN
	1    2100 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 2050 2100 1675
Wire Wire Line
	2100 1675 2700 1675
Connection ~ 2700 1675
Wire Wire Line
	2100 2250 2100 2900
$Comp
L R R1
U 1 1 5FF64AEA
P 2100 3050
F 0 "R1" V 2180 3050 50  0000 C CNN
F 1 "270" V 2100 3050 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2030 3050 50  0001 C CNN
F 3 "" H 2100 3050 50  0001 C CNN
	1    2100 3050
	1    0    0    -1  
$EndComp
Connection ~ 2100 2750
Wire Wire Line
	2100 3200 2100 3400
Text Notes 3725 3600 0    60   ~ 0
Calculations:\n	1) Zener\n		Vt = 5V\n		Iload = 1mA\n		Rmaxload = 1300 ( at about 78 °C according to datasheet )\n\n		Rmaxload = (Vt - Vz)/Iload\n\n		Vz = Vt - Rmaxload*Iload = 3,7 (available is 3,6V - close enough)\n\n	2) Zener resistor\n		Vz = 3,6V\n		Iz = 5mA\n		\n		Rz = (Vt - Vz)/Iz = 280 ohm (availabla is 270 - close enough)
Wire Notes Line
	3700 2125 7075 2125
Wire Notes Line
	3700 2125 3700 3700
Wire Notes Line
	3700 3700 7075 3700
Wire Notes Line
	7075 3700 7075 2125
$EndSCHEMATC

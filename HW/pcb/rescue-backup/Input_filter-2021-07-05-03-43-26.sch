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
Sheet 2 6
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
L C C14
U 1 1 5D1141B8
P 3475 2075
F 0 "C14" H 3500 2175 50  0000 L CNN
F 1 "1nF" H 3500 1975 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 3513 1925 50  0001 C CNN
F 3 "" H 3475 2075 50  0001 C CNN
	1    3475 2075
	1    0    0    -1  
$EndComp
$Comp
L C C15
U 1 1 5D1141DA
P 3700 2075
F 0 "C15" H 3725 2175 50  0000 L CNN
F 1 "10nF" H 3725 1975 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 3738 1925 50  0001 C CNN
F 3 "" H 3700 2075 50  0001 C CNN
	1    3700 2075
	1    0    0    -1  
$EndComp
$Comp
L C C16
U 1 1 5D114200
P 3925 2075
F 0 "C16" H 3950 2175 50  0000 L CNN
F 1 "100nF" H 3950 1975 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 3963 1925 50  0001 C CNN
F 3 "" H 3925 2075 50  0001 C CNN
	1    3925 2075
	1    0    0    -1  
$EndComp
$Comp
L L_Core_Ferrite L2
U 1 1 5D114226
P 3150 1775
F 0 "L2" V 3100 1775 50  0000 C CNN
F 1 "L_Core_Ferrite" V 3260 1775 50  0000 C CNN
F 2 "Inductors_THT:L_Radial_D10.0mm_P5.00mm_Fastron_07M" H 3150 1775 50  0001 C CNN
F 3 "" H 3150 1775 50  0001 C CNN
	1    3150 1775
	0    -1   -1   0   
$EndComp
$Comp
L Q_NMOS_GSD Q1
U 1 1 5D114AF8
P 5250 2250
F 0 "Q1" V 5250 2025 50  0000 L CNN
F 1 "Q_NMOS_GSD" V 5525 1675 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 5450 2350 50  0001 C CNN
F 3 "" H 5250 2250 50  0001 C CNN
	1    5250 2250
	0    -1   1    0   
$EndComp
Text HLabel 5775 2350 2    60   Input ~ 0
GND
Text HLabel 5775 1775 2    60   Input ~ 0
5V
Text HLabel 2800 1775 0    60   Input ~ 0
DC_1
Text HLabel 2800 2350 0    60   Input ~ 0
DC_2
Wire Wire Line
	3925 1775 3925 1925
Connection ~ 3925 1775
Wire Wire Line
	3700 1775 3700 1925
Connection ~ 3700 1775
Wire Wire Line
	3475 1925 3475 1775
Connection ~ 3475 1775
Wire Wire Line
	2800 2350 5050 2350
Wire Wire Line
	3925 2225 3925 2350
Connection ~ 3925 2350
Wire Wire Line
	3700 2225 3700 2350
Connection ~ 3700 2350
Wire Wire Line
	3475 2225 3475 2350
Connection ~ 3475 2350
Wire Wire Line
	5775 2350 5450 2350
Wire Wire Line
	2800 1775 3000 1775
Connection ~ 5250 1775
Wire Wire Line
	5250 1775 5250 2050
$Comp
L D_Zener_Small_ALT D8
U 1 1 5D2FC534
P 4575 1925
F 0 "D8" H 4575 2025 50  0000 C CNN
F 1 "5VBreakdown_Zener" V 4550 2500 50  0000 C CNN
F 2 "Diodes_SMD:D_MiniMELF_Handsoldering" V 4575 1925 50  0001 C CNN
F 3 "" V 4575 1925 50  0001 C CNN
	1    4575 1925
	0    1    1    0   
$EndComp
$Comp
L D_Zener_Small_ALT D9
U 1 1 5D2FC5A9
P 4575 2150
F 0 "D9" H 4575 2050 50  0000 C CNN
F 1 "5VBreakdown_Zener" V 4275 2200 50  0000 C CNN
F 2 "Diodes_SMD:D_MiniMELF_Handsoldering" V 4575 2150 50  0001 C CNN
F 3 "" V 4575 2150 50  0001 C CNN
	1    4575 2150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4575 2050 4575 2025
Wire Wire Line
	4575 2250 4575 2350
Connection ~ 4575 2350
Wire Wire Line
	4575 1775 4575 1825
Connection ~ 4575 1775
$Comp
L Fuse_Small F2
U 1 1 5E87197B
P 4300 1775
F 0 "F2" H 4300 1715 50  0000 C CNN
F 1 "Fuse_Small" H 4300 1835 50  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuseholder_Fuse_TR5_Littlefuse-No560_No460" H 4300 1775 50  0001 C CNN
F 3 "" H 4300 1775 50  0001 C CNN
	1    4300 1775
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 1775 4200 1775
Wire Wire Line
	4400 1775 5775 1775
$EndSCHEMATC

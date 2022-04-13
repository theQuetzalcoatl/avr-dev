EESchema Schematic File Version 4
EELAYER 30 0
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
L panel-rescue:C C14
U 1 1 5D1141B8
P 3375 2125
F 0 "C14" H 3400 2225 50  0000 L CNN
F 1 "1nF" H 3400 2025 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 3413 1975 50  0001 C CNN
F 3 "" H 3375 2125 50  0001 C CNN
	1    3375 2125
	1    0    0    -1  
$EndComp
$Comp
L panel-rescue:C C15
U 1 1 5D1141DA
P 3600 2125
F 0 "C15" H 3625 2225 50  0000 L CNN
F 1 "10nF" H 3625 2025 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 3638 1975 50  0001 C CNN
F 3 "" H 3600 2125 50  0001 C CNN
	1    3600 2125
	1    0    0    -1  
$EndComp
$Comp
L panel-rescue:C C16
U 1 1 5D114200
P 3825 2125
F 0 "C16" H 3850 2225 50  0000 L CNN
F 1 "100nF" H 3850 2025 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 3863 1975 50  0001 C CNN
F 3 "" H 3825 2125 50  0001 C CNN
	1    3825 2125
	1    0    0    -1  
$EndComp
$Comp
L panel-rescue:L_Core_Ferrite L2
U 1 1 5D114226
P 3050 1825
F 0 "L2" V 3000 1825 50  0000 C CNN
F 1 "L_Core_Ferrite" V 3160 1825 50  0000 C CNN
F 2 "Inductors_THT:L_Radial_D10.0mm_P5.00mm_Fastron_07M" H 3050 1825 50  0001 C CNN
F 3 "" H 3050 1825 50  0001 C CNN
	1    3050 1825
	0    -1   -1   0   
$EndComp
$Comp
L panel-rescue:Q_NMOS_GSD Q1
U 1 1 5D114AF8
P 5150 2300
F 0 "Q1" V 5150 2075 50  0000 L CNN
F 1 "Q_NMOS_GSD" V 5425 1725 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 5350 2400 50  0001 C CNN
F 3 "" H 5150 2300 50  0001 C CNN
	1    5150 2300
	0    -1   1    0   
$EndComp
Text HLabel 5675 2400 2    60   Input ~ 0
GND
Text HLabel 5675 1825 2    60   Input ~ 0
5V
Text HLabel 1700 1825 0    60   Input ~ 0
DC_1
Text HLabel 1700 2400 0    60   Input ~ 0
DC_2
Wire Wire Line
	3825 1825 3825 1975
Connection ~ 3825 1825
Wire Wire Line
	3600 1825 3600 1975
Connection ~ 3600 1825
Wire Wire Line
	3375 1975 3375 1825
Connection ~ 3375 1825
Wire Wire Line
	1700 2400 3375 2400
Wire Wire Line
	3825 2275 3825 2400
Connection ~ 3825 2400
Wire Wire Line
	3600 2275 3600 2400
Connection ~ 3600 2400
Wire Wire Line
	3375 2275 3375 2400
Connection ~ 3375 2400
Wire Wire Line
	5675 2400 5350 2400
Wire Wire Line
	1700 1825 2100 1825
Connection ~ 5150 1825
Wire Wire Line
	5150 1825 5150 2100
$Comp
L panel-rescue:D_Zener_Small_ALT D8
U 1 1 5D2FC534
P 4475 2150
F 0 "D8" H 4475 2250 50  0000 C CNN
F 1 "5VBreakdown_Zener" V 4450 2725 50  0000 C CNN
F 2 "Diode_SMD:D_MiniMELF_Handsoldering" V 4475 2150 50  0001 C CNN
F 3 "" V 4475 2150 50  0001 C CNN
	1    4475 2150
	0    1    1    0   
$EndComp
Connection ~ 4475 2400
Wire Wire Line
	4475 1825 4475 2050
Connection ~ 4475 1825
$Comp
L panel-rescue:Fuse_Small F2
U 1 1 5E87197B
P 4200 1825
F 0 "F2" H 4200 1765 50  0000 C CNN
F 1 "Fuse_Small" H 4200 1885 50  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuseholder_Fuse_TR5_Littlefuse-No560_No460" H 4200 1825 50  0001 C CNN
F 3 "" H 4200 1825 50  0001 C CNN
	1    4200 1825
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1825 3375 1825
Wire Wire Line
	4300 1825 4475 1825
Wire Wire Line
	3825 1825 4100 1825
Wire Wire Line
	3600 1825 3825 1825
Wire Wire Line
	3375 1825 3600 1825
Wire Wire Line
	3825 2400 4475 2400
Wire Wire Line
	3600 2400 3825 2400
Wire Wire Line
	3375 2400 3600 2400
Wire Wire Line
	5150 1825 5675 1825
Wire Wire Line
	4475 2400 4950 2400
Wire Wire Line
	4475 1825 5150 1825
$Comp
L Connector:Conn_01x02_Male J10
U 1 1 61EF7CDF
P 2200 1625
F 0 "J10" V 2262 1669 50  0000 L CNN
F 1 "current sense" V 2475 1300 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2200 1625 50  0001 C CNN
F 3 "~" H 2200 1625 50  0001 C CNN
	1    2200 1625
	0    1    1    0   
$EndComp
Wire Wire Line
	2200 1825 2900 1825
Wire Wire Line
	4475 2250 4475 2400
$EndSCHEMATC

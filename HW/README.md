# Hardware

This file is intended to keep track and present the more detailed informations about the HW part of the project.
The PCB is currently developed in KiCAD. From HW side, we do not plan to document everything between releases. E.g. hotfixes, minor changes. Only detailed information will be maintained about the current version of the board, major updates will be tagged and useful information about changes will be listed in it.
---
### HW elements:
* **DAC**: a first order RC low pass filter with variing duty cycle PWM on its input.
* **LCD**: simple display with two rows.
* **Buzzer**: MOSFET switched one, doesn't have internal oscillator, so frequency can be set in SW
* **LEDS & buttons**: 4 leds(blue), two push buttons(B1 - B2)
* **Keypad**: a simple 3 by 4 one
|1|2|3|
|4|5|6|
|7|8|9|
|*|0|#|
* **RTC**: An ATTiny45 will be the RTC and the main MCU will communicate with it via UART
* **UART**:
    * baudrate:
    * bits:
    * etc...
* **Temp sensing**: A PT1000 and a current source will be used
* **Light sensing**: An LDR will be used alongside a regular pot. meter so we can set tresholds for Applications.
* **Capacitive touch sensing**: Two buttons will be realised with only using one MCU pin for each one.
* **Voltage/angle sensing**: A regular pot meter will supply the devided Vcc voltage. It can be turned and as such, can be used as an angle sensor.
* **Rotary encoder**: 

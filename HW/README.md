# Hardware

This file is intended to keep track and present the more detailed informations about the HW part of the project.
The PCB is currently developed in KiCAD. 
---
### HW elements:
* **DAC**: A first order RC low pass filter with variing duty cycle PWM on its input
* **LCD**: A 2x16 display. Backlight can be switched on/off by SW
* **Buzzer**: MOSFET switched, frequency can be set by SW
* **LEDS & buttons**: 4 leds, two push buttons without external pullup resistors
* **Keypad**: a simple 3 by 4 one</br>
      |1|2|3|
      |4|5|6|
      |7|8|9|
      |*|0|#|
* **UART**: Jumper pins for UART  communication, with a suitable connector USB is possible using the CH340G based USB/UART module
* **Temp sensing**: A PT1000 and a current source is used
* **Light sensing**: An LDR is used alongside a regular pot. meter so treshold can also be set
* **Capacitive touch sensing**: Two buttons are made with using only one GPIO each
* **Voltage/angle sensing**: A regular pot meter will supply the devided Vcc voltage. It can be turned and as such, can be used as an angle sensor
* **Rotary encoder**: Capacitors are placed on the MCU pins to help with debouncing

# avr-dev

## General

**Goal:** The project aims to develop a suitable SW architecture and a corresponding HW development board, which can be the foundation of learning and experimentation.
The project will exclusively use AVR microcontrollers.

**Not goal:** Creating something new, better or unique.

---

### Hardware:
An AVR based development board is currently being developed to support the SW features.

The PCB will be a single one and will contain HW elements as:
* LCD
* DAC
* Buzzer
* LEDs & buttons
* Keypad
* UART communication
* Temperature sensing
* Light sensing
* Capacitive touch sensing
* Voltage/angle sensing
* Rotary encoder

More information: [HW](https://github.com/theQuetzalcoatl/avr-dev/blob/master/HW/README.md "Hardware")

### Software:

There will definitely be;
* **A quite primitive OS**
* **Hardware abstraction layers**
* **Board abstraction layers**
And on top of that, the **Application**

The SW architecture will slightly resamble that of the AUTOSAR standard's.

More information: [SW](https://github.com/theQuetzalcoatl/avr-dev/blob/master/SW/README.md "Software")

* [x] Make the corners round
* [x] Add version on silkscreen
* [x] Mark pin 1 of ICs with different pad than the others
* [x] Make all leds consume about 10-15 mA
* [x] Change pins of every button to fit into footprint
* [x] Low ohm resistors to the USART signals, to reduce possible crosstalk
* [-] Elongate the main MCU pads for easier soldering + easier removing of solder bridges.
* [x] Eliminate most of the  90° traces

---------------- v2.0 ----------------------

* [x] Separate Arduino programmer from the board, only SPI connectrions shall remain
* [x] Change the connector pinout to the USB/UART module to have it 5V
* [x] Change position of C17,C9 and L1
* [x] Use one resistor for Tx pin insted of R30 and R12
* [x] Rotary encoder's case shall be grounded
* [x] Led resistors to the back side
* [x] Separate the analogue circuitries from each other.
* [x] Change the GND and VCC planes.
* [x] 5V Jack connector side fuse shall be placed before the zener diodes
* [-] Delete the voltage/angle sensor, a.k.a. pot meter and the one which is used for Analoge Comparison should be moved to one of the ADC multiplexer pins so it can be used for both.
			Additionally, the placement of the pot. meter should be considered as it may get a handle or something. 
* [x] Add Zner diode + resistor to tie the BJT's base to a lower voltage level in order to be able to function as current generator
* [-] Re-order the pinout to the keyboard
* [x] Reverse the order of leds to be able to visually represent binariy counting bits etc.. because right now it is backwards


---------------- v3.0 ----------------------

* [ ] Shrink the ground connection to the PT1000 in order to make it able to represent the *air's* temperature
* [ ] Male headers in series with DC jack input to be able to measure input current.
* [ ] Create a proper footprint for the UART 90° connector.
* [ ] Push buttons shall be SMD
* [ ] Insert LED1..LED4 for leds instead of D4..D7
* [ ] Add a 4 pin male header connected to GND for easier access to GND.

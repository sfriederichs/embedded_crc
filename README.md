#Background#

This repository contains an example of a desktop application and an embedded application that incorporates
CRC generation code generated via PyCRC. The algorithm implemented is CRC-32-MPEG.

The desktop application is designed to be compiled using the MinGW version of GCC. Please see the following article
for specific steps on reproducing this development environment: 

http://www.embeddedrelated.com/showarticle/701.php

The embedded application is designed to be compiled for the ATMega328P microcontroller using AVR-GCC.

The desktop application produces a CRC of a hard-coded file OR a test string (dependent on compiler flags).
The embedded application will CRC data transmitted to it over a UART.

The microcontroller UART is designed to operate at 38.4K.

The microcontroller has a heartbeat on port D pin 7. It should blink at 1Hz.

Upon startup, the microcontroller should transmit the sequence 0xA5,0x5A,0xA5 over the serial port, set port D pin 7 HI, 
wait one second, then set port D, pin 7 low.

The CKDIV8 fuse must be disabled for the embedded application to work.

The embedded application will receive data on the serial port. Once it has received 512 bytes it will calculate the CRC of that data
in three ways:

* Bit-by-Bit 
* Table-driven (table stored in RAM)
* Table-driver (table stored in program memory)

A port pin will (eventually) be set HI at the start of each calculation and then LO when the calculation is finished. 
After the calculation is finished the microcontroller will transmit the result in either big-endian or little-endian
format depending on the TX_LSB_FIRST macro. 
The embedded application will (eventually) wait 10ms (value to be determined experimentally) before performing the same
calculation using a different approach. 

The goal of the embedded application is to compare the time required to calculate the CRC for all three methods.

#Background#

This repository contains an example of a desktop application and an embedded application that incorporates
CRC generation code generated via PyCRC. The algorithm implemented is CRC-32-MPEG.

The desktop application is designed to be compiled using the MinGW version of GCC. Please see the following article
for specific steps on reproducing this development environment: 

http://www.embeddedrelated.com/showarticle/701.php

The embedded application is designed to be compiled for the ATMega328P microcontroller using AVR-GCC.

The desktop application generates the 'check' value of the CRC and then generates the CRC of data/data.bin
The embedded application will do the same but report the CRCs over a serial port. The embedded application also
generates the CRC in four different ways:

* Bit-by-Bit
* Bit-by-Bit-Fast
* Table Driven - Table stored in RAM
* Table-Driven - Table stored in program memory

The microcontroller UART is designed to operate at 38.4K.

The microcontroller has a heartbeat on port D pin 7. It should blink at 1Hz.

Upon startup, the microcontroller should transmit the sequence 0xA5,0x5A,<VER> over the serial port, set port D pin 7 HI, 
wait one second, then set port D, pin 7 low.

<VER> is a version identifier that has no significance beyond debugging.

The CKDIV8 fuse must be disabled for the embedded application to work.

The embedded application profiles the four different CRC algorithms that it uses. It does this by setting pin
Port B pin 0 high when starting the CRC calculation and low when it finishes. There is a 1ms delay between the timing of each algorithm.

For each CRC algorithm, first the check result is calculated and transmitted, then the CRC of the transmitted data and finally, a separator value of '0xFF'. The serial data is transmitted most-significant byte first. 

#Make Targets#

##all##

By simply typing 'make' or 'mingw32-make', the embedded application will be built and programmed. The makefile assumes an Atmel Dragon programmer 
connected over USB to an ATMEGA328PU microcontroller.

##bin/crc.exe##

This target builds the 

#Directory Organization#

##bin##
This directory contains the output HEX or EXE files for the applications.

##data##

This directory contains the data.bin file. This file is randomly-generated and only exists to give something to run CRC algorithms on.

##inc##

This directory includes header files for library functionality - not application header files.

##src##

This directory contains all source files.

##root##

This directory contains:

* README.md - This file
* LICENSE.md - File containing the software license (unlicense AKA public domain)
* makefile - The makefile for this project
* .gitignore - git files to ignore
HEX = bin/crc.hex
EXE = bin/crc.exe
RAND = $(shell set /A test=%RANDOM% * 255 /32768 +1)
TEST = $(shell echo %test%)
all: prgm_target

SRCFILES = src/crc_uc.c src/init.c src/uart.c src/crc_tbl_ram.c src/crc_bbb.c src/crc_bbbf.c src/crc_tbl_pgm.c

HEADERS = src/init.h src/crc_tbl_ram.h src/crc_bbb.h src/crc_bbbf.h src/crc_tbl_pgm.h 

bin/crc.hex: $(SRCFILES) $(HEADERS)
	avr-gcc $(SRCFILES) -Iinc -IC:\WinAVR-20100110\avr\include -mmcu=atmega328p -DF_CPU=8000000 -O1 -Wall -o obj/crc_ram.elf
	avr-objcopy -j .text -j .data -O ihex obj/crc_ram.elf bin/crc.hex

bin/crc.exe: src/crc_desktop.c 
	gcc src/crc_desktop.c  -DUSE_FILE -Wall -o bin/crc.exe
	
prgm_target: $(HEX) 
	avrdude -p m328p -c dragon_isp -P usb -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0x07:m -U flash:w:bin\crc.hex -F
	
FORCE:

clean:
	-rm bin/*.hex
	-rm bin/*.exe

rand:
	echo  $(RAND)
	echo $(TEST)
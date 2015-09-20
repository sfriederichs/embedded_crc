HEX = bin/crc.hex
EXE = bin/crc.exe

bin/crc.hex: src/crc_main.c src/crc_gen.c src/init.c src/init.h src/crc_gen.h
	avr-gcc src/crc_main.c src/init.c src/crc_gen.c -Iinc -IC:\WinAVR-20100110\avr\include -mmcu=atmega328p -DF_CPU=8000000 -O1 -Wall -o obj/crc_ram.elf
	avr-objcopy -j .text -j .data -O ihex obj/crc_ram.elf bin/crc_ram.hex

bin/crc.exe: src/crc_desktop.c 
	gcc src/crc_desktop.c  -DUSE_FILE -Wall -o bin/crc.exe
	
all: $(HEX) $(EXE)

FORCE:

clean:
	-rm bin/*.hex
	-rm bin/*.exe


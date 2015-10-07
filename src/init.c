#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <boolean.h>
#include "init.h"

#define VERSION_ID 12

extern void _delay(int);

statusRtn init(void)
{

	//System Clock Options
	//Desired system clock is:
	// * From an internal source - this is mandatory as I didn't put a crystal on the board
	// * The higher clock frequency, the more performance, however
	// * The lower the frequency, the less power used
	//
	//ATMega328 Datasheet Pg. 27 Section 9.2 - Clock sources are selectable by Flash Fuse bits 
	//ATMega328 Datasheet Pg. 27 Section 9.2.1 - Default clock source is an internal RC oscillator divided by 
	//8 to give a system clock of 1MHz
	//CKDIV8 fuse is disabled, so clock speed is 8MHz
	
	//I/O Time
	
	//Here's my I/O list:
	//
	//	Port	Pin		I/O		Function			Notes
	//-------------------------------------------------------------------------------
	//	D		7		O		Heartbeat LED
	
	//Due to the way the AVR is designed, pins can be configured as General Purpose I/O pins (GPIO) (Section 14.2) or 
	//as their Alternate Port Functions (Section 14.3).
	
	//ATMega328 Datasheet Table 14-1 Pg 78
	//Heartbeat LED - PD7
	//Set as Output High (initially)
	SET(DDRD,7);	//Direction: output
	CLEAR(PORTD,7);	//State: Lo
	
	//Profiling LED - PB0
	//Set as Output High (initially)
	SET(DDRB,0);	//Direction: output
	CLEAR(PORTB,0);	//State: Lo	
	
	PRR = 0x00;	//DIsable all power reduction
	
	//ATMega328 Datasheet Section 16.11.1 pg 132 - TCCR1A
	//No waveform generation is required on this timer, so set all 
	//ports to normal operation
	
	TCCR1A = 0x00;
    
	//ATMega328 Datasheet Section 16.11.3 pg 135 - TCCR1C
	//This register is used for output compare.  Again, we're not doing that
	//So this can be all 0's
	
	TCCR1C = 0x00;
	
	//TCCR1B
	//Note: I've disabled the CKDIV8 fuse so that the Fosc and clock source is 8MHz
	//ATMega328 Datasheet Section 16.11.2 pg 134 - TCCR1A
	//No input capture used - bits 7:6 are 0
	//No waveform generation used - bits 4:3 are 0
	//Clock source select is bits 2:0
	//Timer is either stopped or clocked from either ClkIO or an external source
	//ClkIO is source, 1MHz clock
	//There are prescaler options of 1,8,64,256,1024
	//We need to generate a time of 3s
	//We have 16 bits to work with as well, so a lot of different ways we can do this 
	
	//As per ATMega328 Datasheet Section 16.9.1 page 123, setting the timer to Normal mode causes the counter
	//to count up until it reaches 0xFFFF at which point it will overrun and start back at 0.  To configure this
	//timer/counter to produce a reliable period we need to start counting at a value that causes it to reach
	//0xFFFF in a known period of time.  THe first question is what period of time do we want that to be?
	
	//First question is what is the default? 
	//The default behavior is a prescaler of 1, clock source of 8MHz and a period of 0xFFFF counts or 65566 counts
	//That gives us a period of .125e-6*65536 = ~.008192s = 8.192ms
	
	//A prescaler of 256 gives us 256 times that, roughly 2.1s
	//The clock source is 8000000/256.  This means 1 tick is .125e-6*256 = 32us
	//500ms / .032ms /tick = 1953.125 ticks
	//We can only count an integer number of ticks so we either have to count up to 1953 ticks or 1954 ticks
	//1953 is closest to 1953.125 so we'll use that
	//However, this isn't the value that we will put into the count registers.  We need to count 1953 counts and 
	//be at 0xFFFF for this scheme to work.  So, we need to populate the count register with a value that is 1953
	//less than 0xFFFF.  Some quick math gives us 65535-1953 = 63852 = 0xF85E 
	//Why do I use hex when I could use decimal?  I don't know.  I'm old school.
	
	TCNT1 = TIMER1_PERIOD;
	
	//Now to set the interrupt masks
	//I want no interrupts - we'll poll the overflow flag to determine when an overflow has occurred
	//ATMega328 Datasheet Section 16.11.8 Pg 136 - TIMSK1
	//No interrupts, all 0's
	
	TIMSK1 = 0x00;
	
	//Now the only thing left to do is turn the timer/counter on.  But don't do it yet!  That's always the last
	//thing you do before you start the main loop.  You don't want to miss an overflow situation while you're 
	//initializing other parts of the system.

	//Configure UART for 38400 8N1 Tx Communication
	//We want to transmit accelerometer information for debug purposes
	
	//Step 1 - Baud rate
	//ATMega328 Datasheet Section 20.10 - Table 20-6 pg 192
	//Baud rate settings for fosc of 8MHZ
	//Choosing baud rate of 38.4K for minimum error
	//U2Xn = 0
	//UBRRn = 12
	
	UBRR0 = 12;
	
 	//UCSR0A - UART 0 Control and Status Register A
	//ATMega328 Datasheet Section 20.11.2 pg 194
	//Bits 7:2 - Status bits
	//Bit 1 - Double UART transmission speed - Yes : 1
	//Bit 0 - Multi-Processor Communication Mode - No:0
	
	UCSR0A = 0x00;
	
	//UCSR0B - UART 0 Control and Status Register B
	//ATMega328 Datasheet Section 20.11.3 pg 
	//Bit 7 - Rx Complete Interrupt Enable - 0
	//Bit 6 - Tx Complete Interrupt Enable - 0
	//Bit 5 - USART Data Register Empty interrupt enable - 0
	//Bit 4 - Receiver Enable - Set to 1
	//Bit 3 - Transmitter Enable - Set to 1
	//Bit 2 - Character Size Bit 2 - Set to 0 for 8 bits
	//Bit 1 - 9th receive bit - Ignore
	//Bit 0 - 9th transmit bit - Ignore
	
	UCSR0B = 0x00	| (1 << 3)
					| (1 << 4);
	
	//UCSR0C - UART 0 Control and Status Register C
	//ATMega328 Datasheet Section 20.11.4 - Pg 196
	//BIts 7:6 - Set to asynchronous (clockless) mode: 00
	//Bits 5:4 - Parity setting - None : 00
	//Bit 3 - Stop select - 1 : 0
	//Bit 2:1 - Character size - 8 : 11
	//Bit 0 - Clock polarity: Don't care : 0
	
	UCSR0C = 0x03 << 1;
	
	//Send a known pattern to verify the UART works
	UDR0 = 0xA5;
	
	//Wait until transmit is complete
	while(FALSE == READ(UCSR0A,6));
	
	UDR0 = 0x5A;
	
	while(FALSE == READ(UCSR0A,6));
	
	UDR0 = VERSION_ID;
	
	//Wait until transmit is complete
	while(FALSE == READ(UCSR0A,6));
	
	
	//Flash the LED for a second to show that initialization has successfully occurred
	//For some reason this does not last 1s at all
	SET(PORTD,7);
	_delay_ms(1000);
	CLEAR(PORTD,7);
	
	//ADC all done! Now I can initiate a conversion by setting bit 6 (ADCSC) of ADCSRA to 1 and then polling 
	//bit 3 (ADCIF) until it turns to 1
	
	//Here we can turn the timer on
	//ATMega328 Datasheet Section 16.11.2 Pg 135 - TCCR1B
	//No Waveform generation: bits 4:3 = 0
	//No input capture: bits 7:6 = 0
	//Clock select: ClkIO/256 - bits 2:0 = 100b = 0x04
	
	TCCR1B = 0x04;	//This starts the counter/timer
	
	return SUCCESS;
}
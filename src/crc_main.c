/*
 * @file crc_main.c
 * @brief Example CRC implementation and performance test
 *
 * Created: 11/10/2012 4:05:40 PM
 * @author Stephen Friederichs
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <boolean.h>
#include "init.h"

#include "crc_gen.h"

//System ticks - always important
uint32_t ticks = 0;

int main(void)
{
	//Init
	statusRtn status = init();
	boolean timer_overflow;
	crc_t crc;
	
	crc	= crc_init();
	
	while(SUCCESS == status)
    {
		
		//Determine what events need to be handled this frame
		
		//Handle system timer
		timer_overflow = READ(TIFR1,0);
			
		if(TRUE == timer_overflow)
		{
			SET(TIFR1,0);		//ATMega328 datasheet Section 16.11.9 pg137 - This clears the overflow flag

			TCNT1 = TIMER1_PERIOD;	//Reset the counter period
			ticks++;
			
			#define PERIOD_HB_LED 400
			if(PERIOD_HB_LED == ticks)
			{
				ticks = 0;
				//Signal that this event occurred in this frame
				TOGGLE(PORTD,7);
			}
		}			
		//Handle received serial data
		if(TRUE == (READ(UCSR0A,7)))
		{
			static int32_t rxCount = 0;

			#define CRC_BUFFER_SIZE 512
			static uint8_t crcBuffer[CRC_BUFFER_SIZE];
			
			//Add byte to buffer
			crcBuffer[rxCount++] = UDR0;
			
			if(rxCount == CRC_BUFFER_SIZE)
			{
				//Pin up
				crc = crc_update_tbl_pgm(crc, (const void*)&crcBuffer[0],CRC_BUFFER_SIZE);
				//Pin down
				
				#ifdef TX_LSB_FIRST
				//Transmit CRC result
				UDR0 = (uint8_t)(crc & 0x00FF);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				
				UDR0 = (uint8_t)(crc >>8);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				#else
				UDR0 = (uint8_t)(crc >>8);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				UDR0 = (uint8_t)(crc & 0x00FF);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));

				#endif
				
				//Pin up
				crc = crc_update_tbl_ram(crc, (const void*)&crcBuffer[0],CRC_BUFFER_SIZE);
				//Pin down
				
				#ifdef TX_LSB_FIRST
				//Transmit CRC result
				UDR0 = (uint8_t)(crc & 0x00FF);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				
				UDR0 = (uint8_t)(crc >>8);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				#else
				UDR0 = (uint8_t)(crc >>8);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				UDR0 = (uint8_t)(crc & 0x00FF);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));

				#endif
				
				//Pin up
				crc = crc_update_bbb(crc, (const void*)&crcBuffer[0],CRC_BUFFER_SIZE);
				//Pin down
				crc = crc_finalize_bbb(crc);
				
				#ifdef TX_LSB_FIRST
				//Transmit CRC result
				UDR0 = (uint8_t)(crc & 0x00FF);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				
				UDR0 = (uint8_t)(crc >>8);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				#else
				UDR0 = (uint8_t)(crc >>8);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				UDR0 = (uint8_t)(crc & 0x00FF);
					
				//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));

				#endif
				rxCount=0;
				crc = crc_init();

			}
		}
		status = SUCCESS;		
	}
    //Set LED ACTIVE to signal error
    SET(PORTD,7);
	
    //Loop forever (force user to reset
    for(;;);
}
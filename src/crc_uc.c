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
 /**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 32 bits.
 *****************************************************************************/
typedef uint_fast32_t crc_t;

#include "crc_bbb.h"
#include "crc_bbbf.h"
#include "crc_tbl_ram.h"
#include "crc_tbl_pgm.h"

//System ticks - always important
uint32_t ticks = 0;

int main(void)
{
	//Init
	statusRtn status = init();
	boolean timer_overflow;
	
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
			extern void blkUARTTx(uint8_t x);
			static int32_t rxCount = 0;

			
			#define CRC_BUFFER_SIZE 512
			static uint8_t crcBuffer[CRC_BUFFER_SIZE];
			
			//Add byte to buffer
			crcBuffer[rxCount++] = UDR0;
			
			if(rxCount == CRC_BUFFER_SIZE)
			{
				crc_t crc;
				int8_t i;
				#define TEST_STR_LEN 9
				static const char testString[] = "123456789";
				extern void blkUARTTx(uint8_t x);
				
				crc = crc_init_tbl_pgm();
				crc = crc_update_tbl_pgm(crc, (const void*)&testString[0],TEST_STR_LEN);
				crc = crc_finalize_tbl_pgm(crc);
				
				//Transmit test string CRC
				#ifdef TX_LSB_FIRST
				for(i=0;i<4;i++)
				#else
				for(i=3;i>=0;i--)	
				#endif
				{
					blkUARTTx((uint8_t)((crc>>(i*8)) & 0x000000FF));
				}
				
				crc = crc_init_tbl_pgm();
				SET(PORTB,0);
				crc = crc_update_tbl_pgm(crc, (const void*)&crcBuffer[0],CRC_BUFFER_SIZE);
				crc = crc_finalize_tbl_pgm(crc);
				CLEAR(PORTB,0);
				
				//Transmit test string CRC
				#ifdef TX_LSB_FIRST
				for(i=0;i<4;i++)
				#else
				for(i=3;i>=0;i--)	
				#endif
				{
					blkUARTTx((uint8_t)((crc>>(i*8)) & 0x000000FF));
				}
				_delay_ms(1);
				
				blkUARTTx(0xFF);
				crc = crc_init_tbl_ram();
				crc = crc_update_tbl_ram(crc, (const void*)&testString[0],TEST_STR_LEN);
				crc = crc_finalize_tbl_ram(crc);
				
				//Transmit test string CRC
				#ifdef TX_LSB_FIRST
				for(i=0;i<4;i++)
				#else
				for(i=3;i>=0;i--)	
				#endif
				{
					blkUARTTx((uint8_t)((crc>>(i*8)) & 0x000000FF));
				}
				
				crc = crc_init_tbl_ram();
				SET(PORTB,0);
				crc = crc_update_tbl_ram(crc, (const void*)&crcBuffer[0],CRC_BUFFER_SIZE);
				crc = crc_finalize_tbl_ram(crc);
				CLEAR(PORTB,0);
				
				//Transmit test string CRC
				#ifdef TX_LSB_FIRST
				for(i=0;i<4;i++)
				#else
				for(i=3;i>=0;i--)	
				#endif
				{
					blkUARTTx((uint8_t)((crc>>(i*8)) & 0x000000FF));
				}

				_delay_ms(1);
				blkUARTTx(0xFF);
				crc = crc_init_bbb();
				crc = crc_update_bbb(crc, (const void*)&testString[0],TEST_STR_LEN);
				crc = crc_finalize_bbb(crc);
				
				//Transmit test string CRC
				#ifdef TX_LSB_FIRST
				for(i=0;i<4;i++)
				#else
				for(i=3;i>=0;i--)	
				#endif
				{
					blkUARTTx((uint8_t)((crc>>(i*8)) & 0x000000FF));
				}
				crc = crc_init_bbb();
				SET(PORTB,0);
				crc = crc_update_bbb(crc, (const void*)&crcBuffer[0],CRC_BUFFER_SIZE);
				crc = crc_finalize_bbb(crc);
				CLEAR(PORTB,0);
				
				//Transmit test string CRC
				#ifdef TX_LSB_FIRST
				for(i=0;i<4;i++)
				#else
				for(i=3;i>=0;i--)	
				#endif
				{
					blkUARTTx((uint8_t)((crc>>(i*8)) & 0x000000FF));
				}								
								//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				UDR0 = 0xFF;
				
				_delay_ms(1);
				blkUARTTx(0xFF);
				crc = crc_init_bbbf();
				crc = crc_update_bbbf(crc, (const void*)&testString[0],TEST_STR_LEN);
				crc = crc_finalize_bbbf(crc);
				
				//Transmit test string CRC
				#ifdef TX_LSB_FIRST
				for(i=0;i<4;i++)
				#else
				for(i=3;i>=0;i--)	
				#endif
				{
					blkUARTTx((uint8_t)((crc>>(i*8)) & 0x000000FF));
				}
				crc = crc_init_bbbf();
				SET(PORTB,0);
				crc = crc_update_bbbf(crc, (const void*)&crcBuffer[0],CRC_BUFFER_SIZE);
				crc = crc_finalize_bbbf(crc);
				CLEAR(PORTB,0);
				
				//Transmit test string CRC
				#ifdef TX_LSB_FIRST
				for(i=0;i<4;i++)
				#else
				for(i=3;i>=0;i--)	
				#endif
				{
					blkUARTTx((uint8_t)((crc>>(i*8)) & 0x000000FF));
				}								
								//Wait until transmit is complete
				while(FALSE == READ(UCSR0A,6));
				UDR0 = 0xFF;
				rxCount = 0;

			}
		}
		status = SUCCESS;		
	}
    //Set LED ACTIVE to signal error
    SET(PORTD,7);
	
    //Loop forever (force user to reset
    for(;;);
}
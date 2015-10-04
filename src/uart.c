#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <boolean.h>

#define TX_DELAY 250	//us

void blkUARTTx(uint8_t x)
{
	//Wait until transmit is complete
	UDR0 = x;
	while(FALSE == READ(UCSR0A,6));
	_delay_us(TX_DELAY);
}
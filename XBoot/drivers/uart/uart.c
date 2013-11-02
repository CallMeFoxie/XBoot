/*
 * uart.c
 *
 * Created: 29.12.2012 17:02:03
 *  Author: Ondra
 */ 

#include "uart.h"
#include "../global.h"
#include "../../Conf.h"

#ifdef DEBUG
void uart_init()
{
	DEBUG_UART.CTRLB = USART_TXEN_bm | /*USART_CLK2X_bm | */USART_RXEN_bm;
	DEBUG_UART.CTRLC = USART_CHSIZE_8BIT_gc;
	DEBUG_PORT.DIRSET = (1 << 3);
	
	USARTC0.DATA = 'x';
}

void uart_send(char payload[])
{
	//return;
	while(*payload != 0x00)
	{
		while(!(DEBUG_UART.STATUS & USART_DREIF_bm));

		DEBUG_UART.DATA = *payload++;
	}
}

void uart_println(char payload[])
{
	//return;
	uart_send(payload);
	uart_send("\r\n");
}

void uart_byte(uint8_t val)
{
	while(!(DEBUG_UART.STATUS & USART_DREIF_bm));
	DEBUG_UART.DATA = val;
}

//#ifdef DEBUG
unsigned char map[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '\0' };

void memdump(u8 * ptr, uint8_t length)
{
	//return;
	//while(length-- > 0)
	for(int i = 0; i < length; i++)
	{
		u8 val = ptr[i];
		//uint8_t val = (*ptr) >> 4;
		uart_byte(map[val >> 4]);
		uart_byte(map[val & 0x0F]);
		uart_byte(' ');
	}
}
#endif
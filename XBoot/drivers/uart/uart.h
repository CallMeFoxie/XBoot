/*
 * uart.h
 *
 * Created: 29.12.2012 17:02:23
 *  Author: Ondra
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include "../global.h"

void uart_init();
void uart_send(char payload[]);
void uart_byte(uint8_t val);
void uart_println(char payload[]);
//#ifdef DEBUG
void memdump(u8 * ptr, uint8_t length);
#define usart_u8(var) memdump(&var, 1)
#define usart_u16(var) memdump(&var, 2)
#define usart_u32(var) memdump(&var, 4)
//#endif



#endif /* UART_H_ */
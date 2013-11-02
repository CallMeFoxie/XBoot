/*
 * led.c
 *
 * Created: 3.1.2013 23:18:29
 *  Author: Ondra
 */ 

#include "led.h"

void LED_Init()
{
	PORTR.DIRSET = 0x03;
}

void LED_On(u8 LED)
{
	if(LED == Red)
		PORTR.OUTSET = 0x02;
	else
		PORTR.OUTSET = 0x01;
}

void LED_Off(u8 LED)
{
	if(LED == Red)
		PORTR.OUTCLR = 0x02;
	else
		PORTR.OUTCLR = 0x01;
}
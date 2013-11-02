/*
 * led.h
 *
 * Created: 3.1.2013 23:18:36
 *  Author: Ondra
 */ 


#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#include "../global.h"

void LED_Init();
void LED_On(u8 LED);
void LED_Off(u8 LED);

enum LED {
	Red,
	Orange
};

#endif /* LED_H_ */
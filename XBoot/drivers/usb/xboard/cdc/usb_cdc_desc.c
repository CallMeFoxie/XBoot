/*
 * usb_cdc_desc.c
 *
 * Created: 1.1.2013 22:18:56
 *  Author: Ondra
 */ 

#include "usb_cdc_desc.h"

CDC_Line_Coding_t CDC_Line_Coding = {
	.dwDTERate = 115200,
	.bCharFormat = 0, // 1 stop bit
	.bParityType = 0, // no parity
	.bDataBits = 8 // 8 bits per frame
};

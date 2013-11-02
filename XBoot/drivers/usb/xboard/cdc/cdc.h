/*
 * cdc.h
 *
 * Created: 29.12.2012 17:18:55
 *  Author: Ondra
 */ 


#ifndef CDC_H_
#define CDC_H_

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "../../../global.h"
#include "usb_cdc_desc.h"
#include "../../usb/usb_desc.h"
#include "../usb.h"

/* PUBLIC */
void USB_Serial_Send(byte* Data, u16 Length);
byte USB_Serial_Get();
u8 USB_Serial_Pending();
void USB_Serial_Putchar(char Char);

/* PRIVATE */
void USB_CDC_Setup_Packet(USB_DeviceRequest_t* Request);
void USB_CDC_Data_Packet(u8 Endpoint);

/* STRUCTS */
typedef struct CDC_Status_t
{
	unsigned RTS	: 1;
	unsigned DTR	: 1;
} CDC_Status_t;

void (*CDC_Callback)(void);
void CDC_Callback_Set_Line_Coding(void);


#endif /* CDC_H_ */
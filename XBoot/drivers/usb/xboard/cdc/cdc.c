/*
 * cdc.c
 *
 * Created: 30.12.2012 17:14:33
 *  Author: Ondra
 */ 

#include "cdc.h"
#include "../../../uart/uart.h"
#include <string.h>
#include "../../../../bootloader/bootloader.h"

CDC_Status_t CDC_Status;
byte CDC_Rx_FIFO[256]; // 256 bytes long FIFO buffer
u8 CDC_Rx_FIFORP = 0;
u8 CDC_Rx_FIFOWP = 0;
char PutcharChar;
byte Baudrate_Reply = CMD_NOP_REPLY;

void USB_CDC_Setup_Packet(USB_DeviceRequest_t* Request)
{
	#ifdef DEBUG
	memdump(Request, sizeof(USB_DeviceRequest_t));
	uart_println("");
	#endif
	switch(Request->bRequest)
	{
		case CDC_GET_LINE_CODING:
			USB_Send_Data(EP_CTRL_IN, &CDC_Line_Coding, sizeof(CDC_Line_Coding_t));
			USB_Ack(EP_CTRL_IN);
			USB_Ack(EP_CTRL_OUT);
			#ifdef DEBUG
			uart_println("CDC: Get line coding");
			#endif
		break;
		case CDC_SET_CONTROL_LINE_STATE:
			USB_ZLP(EP_CTRL_IN);
			USB_Ack(EP_CTRL_IN);
			USB_Ack(EP_CTRL_OUT);
			#ifdef DEBUG
			uart_println("CDC: Set control line state");
			#endif
			
		break;
		case CDC_SET_LINE_CODING:
			//USB_ZLP(EP_CTRL_IN);
			//USB_Ack(EP_CTRL_IN);
			USB_Ack(EP_CTRL_OUT);
			#ifdef DEBUG
			uart_println("CDC: Set Line Coding");
			#endif
			CDC_Callback = &CDC_Callback_Set_Line_Coding;
		break;
	}
	
	if(Request->wLength == 0)
	{
		//USB_ZLP(EP_CTRL_IN);
		//USB_Ack(EP_CTRL_IN);
		//USB_Ack(EP_CTRL_OUT);
	}
}

void USB_CDC_Data_Packet(u8 Endpoint)
{
	if(Endpoint == EP_DATA_OUT)
	{
		#ifdef DEBUG
		uart_send("Received data: ");
		memdump(USB_Get_DATA(EP_DATA_OUT), USB_Get_CNT(EP_DATA_OUT));
		uart_println(" ");
		#endif
		
		u8 CNT = USB_Get_CNT(EP_DATA_OUT);
		u16 *dataptr = USB_Get_DATA(EP_DATA_OUT);
		
		if(CDC_Line_Coding.dwDTERate != BL_BAUDRATE)
		{
			for(int i = 0; i < CNT; i++)
			{
				CDC_Rx_FIFO[CDC_Rx_FIFOWP++] = *dataptr++;
			}
		}
		else
		{
			for(int i = 0; i < CNT; i++)
			{
				#ifdef DEBUG
				uart_println("bootloader 1200 baud");
				#endif
				USB_Serial_Send(&Baudrate_Reply, 1);
			}
		}
		
		USB_Ack(EP_DATA_OUT);
	}
}

void USB_Serial_Send(byte* Data, u16 Length)
{
	if(Length > 1023)
		return;
		
	USB_Send_Data(EP_DATA_IN, Data, Length);
	USB_Ack(EP_DATA_IN);
	
	while(!USB_Is_Completed(EP_DATA_IN)); // wait for the data to be sent. If we were to "background" it, we would need another 1023B memory on heap. Per incoming transaction. Simpler this way (all data on stack)
}

void USB_Serial_Putchar(char Char)
{
	PutcharChar = Char;
	USB_Serial_Send(&PutcharChar, 1);
}

byte USB_Serial_Get()
{
	
	if(CDC_Rx_FIFORP != CDC_Rx_FIFOWP)
	{
		USB_Clear_Stall(EP_DATA_OUT);
		return CDC_Rx_FIFO[CDC_Rx_FIFORP++];
	}		
	
	return NULL;
	
}

u8 USB_Serial_Pending()
{
	return CDC_Rx_FIFOWP - CDC_Rx_FIFORP;
	//return 0;
}

void CDC_Callback_Set_Line_Coding(void)
{
	memcpy(&CDC_Line_Coding, USB_Get_DATA(EP_CTRL_OUT), sizeof(CDC_Line_Coding_t));
	#ifdef DEBUG
	uart_send("Setting new baud rate to ");
	memdump(&CDC_Line_Coding, sizeof(CDC_Line_Coding_t));
	uart_println("");
	#endif
	USB_Ack(EP_CTRL_OUT);
	USB_ZLP(EP_CTRL_IN);
	USB_Ack(EP_CTRL_IN);
	CDC_Callback = NULL;
}
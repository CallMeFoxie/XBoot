/*
 * usb.h
 *
 * Created: 29.12.2012 17:09:49
 *  Author: Ondra
 */ 


#ifndef USB_H_
#define USB_H_

/*
Endpoints:
+---------------------------------------+
| X | R | DIR | DESC                    |
+---------------------------------------+
| 0 | 0 | OUT | CTRL                    |
| 0 | 1	| IN  | CTRL                    |
| 1	| 2	| --  | NOT USED                |
| 1	| 3	| IN  | DATA	bulk (UART TX)  |
| 2	| 4	| OUT | DATA	bulk (UART RX)  |
| 2	| 5	| IN  | NOTIF	interrupt       |
+---------------------------------------+
*/

#define XBOOT_ENDPOINTS	3

#define EP_CTRL_OUT		0
#define EP_CTRL_IN		1
#define EP_DATA_IN		3
#define EP_DATA_OUT		4
#define EP_NOTIF_OUT	5

#include <avr/interrupt.h>
#include <avr/io.h>
#include "usb_desc.h"
#include "../../../Conf.h"
#include "../../global.h"
//#include "cdc/cdc.h"
#include "../../pmic/pmic.h"
#include "../../clock/clock.h"

/* PUBLIC */
void USB_Init();
void USB_Reset();
void USB_Attach();
void USB_Detach();
void USB_Vect_BUSEVENT();
void USB_Vect_TRNCOMPL();

/* PRIVATE */
void USB_Set_Registers();
void USB_Process_Setup_Packet();
void USB_Reset_Everything();
void USB_Fetch_Descriptor(USB_DeviceRequest_t* Packet);


/* QUICK FUNCTIONS */
#define USB_Endpoint_Set_CTRL(ep, val)		usb_ram.endpoint[ep].CTRL = val
#define USB_Endpoint_Reset(ep)				usb_ram.endpoint[ep].STATUS = 0; \
											usb_ram.endpoint[ep].AUXDATA = 0; \
											usb_ram.endpoint[ep].CNT = 0; \
											usb_ram.endpoint[ep].CTRL = 0
#define USB_Ack_TRNCOMPL()					USB.INTFLAGSBCLR = USB_TRNIF_bm
#define USB_Is_Setup_Packet()				USB.INTFLAGSBSET & USB_SETUPIF_bm
#define USB_Ack_Setup_Packet()				USB.INTFLAGSBCLR = USB_SETUPIF_bm
#define USB_Get_EP_From_FIFO()				((u16*)((u16)usb_ram.endpoint - 2 * (1 + ~USB.FIFORP)) - (((u16)usb_ram.endpoint) >> 3))
#define USB_Set_Address(addr)				USB.ADDR = addr
#define USB_Set_AUXDATA(ep, val)			usb_ram.endpoint[ep].AUXDATA = val
#define USB_Set_CNT(ep, val)				usb_ram.endpoint[ep].CNT = val
#define USB_Set_DATA(ep, addr)				usb_ram.endpoint[ep].DATAPTR = addr
#define USB_Get_AUXDATA(ep)					usb_ram.endpoint[ep].AUXDATA
#define USB_Get_CNT(ep)						usb_ram.endpoint[ep].CNT
#define USB_Get_DATA(ep)					usb_ram.endpoint[ep].DATAPTR
#define USB_ZLP(ep)							usb_ram.endpoint[ep].CNT = 0; \
											usb_ram.endpoint[ep].AUXDATA = 0;
#define USB_Ack(ep)							LACR(&usb_ram.endpoint[ep].STATUS, USB_EP_BUSNACK0_bm)
#define USB_Send_Data(ep, ptr, length)		USB_Set_AUXDATA(ep, length); \
											USB_Set_CNT(ep, MIN(XBOOT_DATASIZE, length)); \
											USB_Set_DATA(ep, ptr);
#define USB_Clear_Stall(ep)					LACR(&usb_ram.endpoint[ep].STATUS, USB_EP_STALLF_bm)
#define USB_EP_Ack_Setup(ep)				LACR(&usb_ram.endpoint[ep].STATUS, USB_EP_SETUP_bm)
#define USB_Ack_Suspend()					USB.INTFLAGSACLR = USB_SUSPENDIF_bm
#define USB_Ack_Resume()					USB.INTFLAGSACLR = USB_RESUMEIF_bm
#define USB_Is_Completed(ep)				(usb_ram.endpoint[0].STATUS & USB_EP_TRNCOMPL0_bm)

/* ENUMS */
enum USB_States
{
	Powered,
	AddressPending,
	Addressed,
	Configured
};

uint16_t USB_New_Addr;
volatile byte USB_State;

/* STRUCTS */
typedef struct
{
	u8 padding;
	u32 FIFO[(XBOOT_ENDPOINTS + 1)];
	//;
	USB_EP_t endpoint[XBOOT_ENDPOINTS * 2];
	u16 FrameNumber;
	unsigned char payload[XBOOT_ENDPOINTS][64]; // we only need half of the endpoints ("OUT")
}  USB_RAM_t;

__attribute__((__aligned__(16))) volatile USB_RAM_t usb_ram;

#endif /* USB_H_ */
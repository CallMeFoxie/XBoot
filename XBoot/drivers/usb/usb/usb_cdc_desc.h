/*
 * usb_cdc.h
 *
 * Created: 29.12.2012 17:09:30
 *  Author: Ondra
 */ 


#ifndef USB_CDC_H_
#define USB_CDC_H_

#include "../../global.h"

#define USB_CDC_Device_Class	0x02
#define USB_CDC_Interface_Class	0x02

#define USB_CDC_Interface_Data_Class	0x0A

#define USB_CDC_Subclass_DirectLineControlModel			0x01
#define USB_CDC_Subclass_AbstractControlModel			0x02
#define USB_CDC_Subclass_TelephoneControlModel			0x03
#define USB_CDC_Subclass_MultichannelControlModel		0x04
#define USB_CDC_Subclass_CAPIControlModel				0x05
#define USB_CDC_Subclass_EthernetNetworkingControlModel	0x06
#define USB_CDC_Subclass_ATMNetworkingControlModel		0x07
#define USB_CDC_Subclass_WirelessHandsetControlModel	0x08
#define USB_CDC_Subclass_DeviceManagement				0x09
#define USB_CDC_Subclass_MobileDirectLineModel			0x0A
#define USB_CDC_Subclass_OBEX							0x0B
#define USB_CDC_Subclass_EthernetEmulationModel			0x0C
#define USB_CDC_Subclass_NetworkControlModel			0x0D

#define USB_CDC_Interface_Class_Protocol_USB				0x00
#define USB_CDC_Interface_Class_Protocol_ITU_T_V250			0x01
#define USB_CDC_Interface_Class_Protocol_PCCA101			0x02
#define USB_CDC_Interface_Class_Protocol_PCCA101_ANNEX_O	0x03
#define USB_CDC_Interface_Class_Protocol_GSM				0x04
#define USB_CDC_Interface_Class_Protocol_3GPP				0x05
#define USB_CDC_Interface_Class_Protocol_CDMA				0x06
#define USB_CDC_Interface_Class_Protocol_USB_EEM			0x07

#define USB_CDC_Interface_Data_Class					0x0A
#define USB_CDC_Interface_SubClass						0x00

#define  CDC_CS_INTERFACE     0x24
#define  CDC_CS_ENDPOINT      0x25

#define  CDC_SCS_HEADER       0x00
#define  CDC_SCS_CALL_MGMT    0x01
#define  CDC_SCS_ACM          0x02
#define  CDC_SCS_UNION        0x06

#define  CDC_ACM_SUPPORT_LINE_REQUESTS       (1 << 1)
#define  CDC_ACM_SUPPORT_SENDBREAK_REQUESTS  (1 << 2)
#define  CDC_ACM_SUPPORT_NOTIFY_REQUESTS     (1 << 3)

#define  CDC_CALL_MGMT_SUPPORTED             (1 << 0)
#define  CDC_CALL_MGMT_OVER_DCI              (1 << 1)

// PSTN subclass specific requests
#define	CDC_SET_COMM_FEATURE			0x02
#define CDC_GET_COMM_FEATURE			0x03
#define CDC_CLEAR_COMM_FEATURE			0x04
#define CDC_SET_AUX_LINE_STATE			0x10
#define CDC_SET_HOOK_STATE				0x11
#define CDC_PULSE_SETUP					0x12
#define CDC_SEND_PULSE					0x13
#define CDC_SET_PULSE_TIME				0x14
#define CDC_RING_AUX_JACK				0x15
#define CDC_SET_LINE_CODING				0x20
#define CDC_GET_LINE_CODING				0x21
#define CDC_SET_CONTROL_LINE_STATE		0x22
#define CDC_SEND_BREAK					0x23
#define CDC_SET_RINGER_PARMS			0x30
#define CDC_GET_RINGER_PARMS			0x31
#define CDC_SET_OPERATION_PARMS			0x32
#define CDC_GET_OPERATION_PARMS			0x33
#define CDC_SET_LINE_PARMS				0x34
#define CDC_GET_LINE_PARMS				0x35
#define CDC_DIAL_DIGITS					0x36


/* STRUCTS */
typedef struct CDC_Line_Coding_t
{
	dword dwDTERate;
	byte bCharFormat;
	byte bParityType;
	byte bDataBits;
} CDC_Line_Coding_t;

#define CDC_CONTROL_STATE_RTS_MASK		1 << 1
#define CDC_CONTROL_STATE_DTR_MASK		1 << 0


#endif /* USB_CDC_H_ */
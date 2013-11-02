/*
 * usb_desc.h
 *
 * Created: 29.12.2012 17:09:39
 *  Author: Ondra
 */ 


#ifndef _USB_DESC_H_
#define _USB_DESC_H_

#include <stddef.h>
#include "../../global.h"
#include "usb_cdc_desc.h"

#define USB_Request_GET_STATUS			0
#define USB_Request_CLEAR_FEATURE		1
#define USB_Request_SET_FEATURE			3
#define USB_Request_SET_ADDRESS			5
#define USB_Request_GET_DESCRIPTOR		6
#define USB_Request_SET_DESCRIPTOR		7
#define USB_Request_GET_CONFIGURATION	8
#define USB_Request_SET_CONFIGURATION	9
#define USB_Request_GET_INTERFACE		10
#define USB_Request_SET_INTERFACE		11
#define USB_Request_SYNCH_FRAME			12

#define USB_Request_Descriptor_DEVICE						1
#define USB_Request_Descriptor_CONFIGURATION				2
#define USB_Request_Descriptor_STRING						3
#define USB_Request_Descriptor_INTERFACE					4
#define USB_Request_Descriptor_ENDPOINT						5
#define USB_Request_Descriptor_DEVICE_QUALIFIER				6
#define USB_Request_Descriptor_OTHER_SPEED_CONFIGURATION	7
#define USB_Request_Descriptor_INTERFACE_POWER				8

#define USB_Request_Direction_MASK		0b1		<< 7
#define USB_Request_Type_MASK			0b11	<< 5
#define USB_Request_Recipient_MASK		0b11111	<< 0

#define USB_Request_Direction_HOSTTODEVICE		0 << 7
#define USB_Request_Direction_DEVICETOHOST		1 << 7

#define USB_Request_Type_STANDARD		0 << 5
#define USB_Request_Type_CLASS			1 << 5
#define USB_Request_Type_VENDOR			2 << 5

#define USB_Request_Recipient_DEVICE	0 << 0
#define USB_Request_Recipient_INTERFACE	1 << 0
#define USB_Request_Recipient_ENDPOINT	2 << 0
#define USB_Request_Recipient_OTHER		3 << 0

#define USB_CONF_SELF_POWERED	(1 << 6)
#define USB_CONF_REMOTE_WAKEUP	(1 << 5)
#define USB_CONF_ALWAYSONE		(1 << 7)

#define USB_Endpoint_Type_Control		0
#define USB_Endpoint_Type_Isochronous	1
#define USB_Endpoint_Type_Bulk			2
#define USB_Endpoint_Type_Interrupt		3



typedef struct Device_Descriptor_t
{
	byte bLength;
	byte bDescriptorType;
	word bcdUSB;
	byte bDeviceClass;
	byte bDeviceSubClass;
	byte bDeviceProtocol;
	byte bMaxPacketSize0;
	word idVendor;
	word idProduct;
	word bcdDevice;
	byte iManufacturer;
	byte iProduct;
	byte iSerialNumber;
	byte bNumConfigurations;
} Device_Descriptor_t;

typedef struct Device_Qualifier_Descriptor_t
{
	byte bLength;
	byte bDescriptorType;
	word bcdUSB;
	byte bDeviceClass;
	byte bDeviceSubClass;
	byte bDeviceProtocol;
	byte bMaxPacketSize0;
	byte bNumConfigurations;
	byte bReserved;
} Device_Qualifier_Descriptor_t;

typedef struct String_Descriptor_t
{
	byte bLength;
	byte bDescriptorType;
	wchar_t bString[24];
} String_Descriptor_t;

typedef struct Configuration_Descriptor_t
{
	byte bLength;
	byte bDescriptorType;
	word wTotalLength;
	byte bNumInterfaces;
	byte bConfigurationValue;
	byte iConfiguration;
	byte bmAttributes;
	byte bMaxPower;
} Configuration_Descriptor_t;

typedef struct Interface_Descriptor_t
{
	byte bLength;
	byte bDescriptorType;
	byte bInterfaceNumber;
	byte bAlternateSetting;
	byte bNumEndpoints;
	byte bInterfaceClass;
	byte bInterfaceSubClass;
	byte bInterfaceProtocol;
	byte iInterface;
} Interface_Descriptor_t;

typedef struct Endpoint_Descriptor_t
{
	byte bLength;
	byte bDescriptorType;
	byte bEndpointAddress;
	byte bmAttributes;
	word wMaxPacketSize;
	byte bInterval;
} Endpoint_Descriptor_t;

typedef struct Language_Descriptor_t
{
	byte bLength;
	byte bDescriptorType;
	word wLANGID;
} Language_Descriptor_t;

typedef struct Get_Status_t
{
	byte b1;
	byte b2;
} Get_Status_t;

typedef struct {
	byte bFunctionLength;
	byte bDescriptorType;
	byte bDescriptorSubtype;
	word bcdCDC;
} CDC_Header_t;

typedef struct {
	byte bFunctionLength;
	byte bDescriptorType;
	byte bDescriptorSubtype;
	byte bmCapabilities;
} CDC_ACM_t;

typedef struct {
	byte bFunctionLength;
	byte bDescriptorType;
	byte bDescriptorSubtype;
	byte bControlInterface;
	byte bSubordinateInterface0;
} CDC_Union_t;

typedef struct {
	byte bFunctionLength;
	byte bDescriptorType;
	byte bDescriptorSubtype;
	byte bmCapabilities;
	byte bDataInterface;
} CDC_Management_t;

typedef struct {
	Interface_Descriptor_t Interface;
	CDC_Header_t Header;
	CDC_ACM_t ACM;
	CDC_Union_t Union;
	CDC_Management_t Management;
	Endpoint_Descriptor_t EndpointNotify;
} USB_CDC_Descriptor_Communication_t;

typedef struct {
	Interface_Descriptor_t Interface;
	Endpoint_Descriptor_t EndpointIn;
	Endpoint_Descriptor_t EndpointOut;
} USB_CDC_Descriptor_Data_t;


typedef struct Complete_Configuration_t
{
	Configuration_Descriptor_t Configuration;
	USB_CDC_Descriptor_Communication_t Comm;
	USB_CDC_Descriptor_Data_t Data;
} Complete_Configuration_t;

typedef struct {
	Configuration_Descriptor_t Configuration;
	USB_CDC_Descriptor_Communication_t Communication;
	USB_CDC_Descriptor_Data_t Data;
} UDC_Descriptor_t;

typedef struct
{
	byte bLength;
	byte bDescriptorType;
	word wTotalLength;
	byte bNumInterfaces;
	byte bConfigurationValue;
	byte iConfiguration;
	byte bmAttributes;
	byte bMaxPower;
} Other_Speed_Configuration_t;


typedef struct USB_DeviceRequest_t
{
	byte bmRequestType;
	byte bRequest;
	word wValue;
	word wIndex;
	word wLength;
} USB_DeviceRequest_t;



#endif /* _USB_DESC_H_ */
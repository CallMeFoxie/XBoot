/*
 * usb_desc.h
 *
 * Created: 29.12.2012 17:16:45
 *  Author: Ondra
 */ 

#include "usb_desc.h"

Device_Descriptor_t USB_Descriptor_Device = {
	.bLength = sizeof(Device_Descriptor_t),
	.bDescriptorType = USB_Request_Descriptor_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = USB_CDC_Device_Class,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = XBOOT_DATASIZE,
	.idVendor = XBOOT_VID,
	.idProduct = XBOOT_PID,
	.bcdDevice = XBOOT_VERSION,
	.iManufacturer = USB_STRING_MANUFACTURER,
	.iProduct = USB_STRING_PRODUCT,
	.iSerialNumber = 0,
	.bNumConfigurations = 1
};

String_Descriptor_t USB_Descriptor_String_Manufacturer = {
	.bLength = sizeof(XBOOT_MANUFACTURER),
	.bDescriptorType = USB_Request_Descriptor_STRING,
	.bString = XBOOT_MANUFACTURER
};

String_Descriptor_t USB_Descriptor_String_Product = {
	.bLength = sizeof(XBOOT_PRODUCT),
	.bDescriptorType = USB_Request_Descriptor_STRING,
	.bString = XBOOT_PRODUCT
};

Language_Descriptor_t USB_Descriptor_Language = {
	.bLength = 4, // fixed..
	.bDescriptorType = USB_Request_Descriptor_STRING,
	.wLANGID = 0x0409
};

Complete_Configuration_t USB_Descriptor_Configuration = {
	.Configuration = {
		.bLength = sizeof(Configuration_Descriptor_t),
		.bDescriptorType = USB_Request_Descriptor_CONFIGURATION,
		.wTotalLength = sizeof(Complete_Configuration_t),
		.bNumInterfaces = 2,
		.bConfigurationValue = 1,
		.iConfiguration = 0,
		.bmAttributes = USB_CONF_ALWAYSONE,
		.bMaxPower = 500/2
	},
	.Comm = {
		.Interface = {
			.bLength = sizeof(Interface_Descriptor_t),
			.bDescriptorType = USB_Request_Descriptor_INTERFACE,
			.bInterfaceNumber = 0,
			.bAlternateSetting = 0,
			.bNumEndpoints = 1,
			.bInterfaceClass = USB_CDC_Device_Class,
			.bInterfaceSubClass = USB_CDC_Subclass_AbstractControlModel,
			.bInterfaceProtocol = USB_CDC_Interface_Class_Protocol_ITU_T_V250,
			.iInterface = 0
		},
		.Header = {
			.bFunctionLength = sizeof(CDC_Header_t),
			.bDescriptorType = CDC_CS_INTERFACE,
			.bDescriptorSubtype = CDC_SCS_HEADER,
			.bcdCDC = 0x0110
		},
		.ACM = {
			.bFunctionLength = sizeof(CDC_ACM_t),
			.bDescriptorType = CDC_CS_INTERFACE,
			.bDescriptorSubtype = CDC_SCS_ACM,
			.bmCapabilities = CDC_ACM_SUPPORT_LINE_REQUESTS
			
		},
		.Union = {
			.bFunctionLength = sizeof(CDC_Union_t),
			.bDescriptorType = CDC_CS_INTERFACE,
			.bDescriptorSubtype = CDC_SCS_UNION,
			.bSubordinateInterface0 = 01 //CDC Data
		},
		.Management = {
			.bFunctionLength = sizeof(CDC_Management_t),
			.bDescriptorType = CDC_CS_INTERFACE,
			.bDescriptorSubtype = CDC_SCS_CALL_MGMT,
			.bmCapabilities = CDC_CALL_MGMT_OVER_DCI | CDC_CALL_MGMT_SUPPORTED,
			.bDataInterface = 1
		},
		.EndpointNotify = {
			.bLength = sizeof(Endpoint_Descriptor_t),
			.bDescriptorType = USB_Request_Descriptor_ENDPOINT,
			.bmAttributes = USB_Endpoint_Type_Interrupt,
			.wMaxPacketSize = XBOOT_DATASIZE,
			.bInterval = 0x10,
			.bEndpointAddress = 0x82
		}
	},
	.Data = {
		.Interface = {
			.bLength = sizeof(Interface_Descriptor_t),
			.bDescriptorType = USB_Request_Descriptor_INTERFACE,
			.bAlternateSetting = 0,
			.bNumEndpoints = 2,
			.bInterfaceClass = USB_CDC_Interface_Data_Class,
			.bInterfaceSubClass = 0,
			.bInterfaceProtocol = 0,
			.bInterfaceNumber = 1,
			.iInterface = 0
		},
		.EndpointIn = {
			.bLength = sizeof(Endpoint_Descriptor_t),
			.bDescriptorType = USB_Request_Descriptor_ENDPOINT,
			.bmAttributes = USB_Endpoint_Type_Bulk,
			.bInterval = 0,
			.bEndpointAddress = 0x81,
			.wMaxPacketSize = XBOOT_DATASIZE
		},
		.EndpointOut = {
			.bLength = sizeof(Endpoint_Descriptor_t),
			.bDescriptorType = USB_Request_Descriptor_ENDPOINT,
			.bmAttributes = USB_Endpoint_Type_Bulk,
			.bInterval = 0,
			.bEndpointAddress = 0x02,
			.wMaxPacketSize = XBOOT_DATASIZE
		}
	}
};

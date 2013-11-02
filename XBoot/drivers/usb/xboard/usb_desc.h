/*
 * usb_desc.h
 *
 * Created: 1.1.2013 22:16:06
 *  Author: Ondra
 */ 


#ifndef XBOOT_USB_DESC_H_
#define XBOOT_USB_DESC_H_

#include "../usb/usb_desc.h"
#include "../usb/usb_cdc_desc.h"
#include "../../../Conf.h"

Device_Descriptor_t USB_Descriptor_Device;
String_Descriptor_t USB_Descriptor_String_Manufacturer;
String_Descriptor_t USB_Descriptor_String_Product;
Language_Descriptor_t USB_Descriptor_Language;
Complete_Configuration_t USB_Descriptor_Configuration;


#endif /* XBOOT_USB_DESC_H_ */
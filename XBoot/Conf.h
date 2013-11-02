/*
 * Conf.h
 *
 * Created: 1.1.2013 20:26:58
 *  Author: Ondra
 */ 


#ifndef CONF_H_
#define CONF_H_

#define XBOOT_MANUFACTURER	L"XBoard"
#define XBOOT_PRODUCT		L"XBoard Mini"
#define XBOOT_SN			L"0001"

#define _XBOARD_MINI_

#define XBOOT_VID		0x03EB
#define XBOOT_PID		0x2404
#define XBOOT_VERSION	0x0100

#define USB_STRING_LIST				0
#define USB_STRING_MANUFACTURER		1
#define USB_STRING_PRODUCT			2
#define USB_STRING_SERIAL			3

#define XBOOT_DATASIZE	64

#define DEBUG_UART		USARTE0
#define DEBUG_PORT		PORTE

#define BL_MIN_MEM		1024 // 1kB allocated for BL. If they use their own USB stack, they can use even this 1kB

#define BL_MAGIC		0x8EA5

#define DEBUG			1

#define BL_BAUDRATE		1200 // baudrate on which BL will be communicating


#endif /* CONF_H_ */
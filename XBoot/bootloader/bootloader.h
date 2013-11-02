/*
 * bootloader.h
 *
 * Created: 10.1.2013 23:07:35
 *  Author: Ondra
 */ 


#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include "../drivers/global.h"
#include "../drivers/usb/xboard/cdc/cdc.h"
#include "../drivers/crc/crc.h"
#include "../board/board.h"
#include "../drivers/uart/uart.h"
#include "../drivers/nvm/nvm.h"
#include <avr/io.h>

void LaunchBootloader();
void PrepareForApplication();


/* STRUCTS */
typedef struct BOOTLOADER_DESCRIPTORS
{
	u16 Stack_Top;
	void (*USB_Attach)(void);
	void (*USB_Detach)(void);
	void (*USB_Vect_BUSEVENT)(void);
	void (*USB_Vect_TRNCOMPL)(void);
} BOOTLOADER_DESCRIPTORS;

typedef struct CMD_GETID_REPLY_t
{
	byte Board;
	word BootloaderVersion;
	word PageSize;
	word FlashSize;
} CMD_GETID_REPLY_t;

typedef struct CMD_PAGE_t
{
	word PageNumber;
	byte Part;
	word CRCData;
	byte Payload[32];
	word Magic; // will be 00 00 for CMD_PAGE, otherwise special key for CMD_BLPAGE
} CMD_PAGE_t;

enum Bootloader_Commands {
	CMD_NOP = 0x00,
	CMD_GETID = 0x01,
	CMD_RESET = 0x02,
	CMD_APP = 0x03,
	CMD_PAGE = 0x04,
	CMD_WRITE = 0x05,
	
	CMD_BLDNVM = 0x80,
	CMD_BLWRITE = 0x81
};

/*enum Bootloader_Commands {
	CMD_NOP = 'x',
	CMD_GETID = 'a',
	CMD_RESET = 'b',
	CMD_APP = 'c',
	CMD_PAGE = 'd',
	CMD_WRITE = 'e',
	CMD_BLPAGE = 'f',
	CMD_BLDNVM = 'g',
	CMD_BLWRITE = 'h'
};*/

#define CMD_NOP_REPLY	0x5A

void FetchPage(CMD_PAGE_t *Page);


#endif /* BOOTLOADER_H_ */
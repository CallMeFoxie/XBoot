/*
 * bootloader.c
 *
 * Created: 10.1.2013 23:07:42
 *  Author: Ondra
 */ 

#include "bootloader.h"
#include "../selfupdater/selfupdater.h"

/*
CMD_GETID_REPLY_t GetID_Reply = {
	.Board = BOARDID,
	.BootloaderVersion = XBOOT_VERSION,
	.PageSize = PAGESIZE * 2,
	.FlashSize = FLASHAPP
};
*/

CMD_GETID_REPLY_t GetID_Reply = {
	.Board = 'a',
	.BootloaderVersion = 'b',
	.PageSize = '5',
	.FlashSize = '6'
};

void LaunchBootloader()
{
	char Keep = 1;
	u16 PageNumber = 0;
	
	CMD_PAGE_t Page;
	
	u16 Magic;
	
	while(Keep)
	{
		while(USB_Serial_Pending() == 0);
		
		byte Command = USB_Serial_Get();
		
		switch(Command)
		{
			case CMD_NOP:
				#ifdef DEBUG
				uart_println("CMD_NOP");
				#endif
				USB_Serial_Putchar(CMD_NOP_REPLY);
			break;
			case CMD_GETID:
				#ifdef DEBUG
				uart_println("CMD_GETID");
				#endif
				USB_Serial_Send(&GetID_Reply, sizeof(CMD_GETID_REPLY_t));
			break;
			
			case CMD_RESET:
				#ifdef DEBUG
				uart_println("CMD_RESET");
				#endif
				CCP = CCP_IOREG_gc;
				RST.CTRL = RST_SWRST_bm;
			break;
			
			case CMD_PAGE:
				#ifdef DEBUG
				uart_println("CMD_PAGE.");
				#endif
				
				FetchPage(&Page);
				
				#ifdef DEBUG
				uart_println("CMD_PAGE. Fetched successfully.");
				#endif
				
				// check CRC
				CRC_Init();
				for(int i = 0; i < 32; i++)
				{
					CRC_Add(Page.Payload[i]);
				}
				
				uart_send("Page Number: ");
				memdump(&Page.PageNumber, 2);
				uart_send("Part: ");
				memdump(&Page.Part, 1);
				uart_send("CRC16: ");
				memdump(&Page.CRCData, 2);
				
				u16 CheckCRC = CRC_Result16();
				uart_send("Expected: ");
				memdump(&CheckCRC, 2);
				uart_println("");
				
				if(CheckCRC != Page.CRCData)
				{
					USB_Serial_Putchar(0);
					#ifdef DEBUG
					uart_println("CMD_PAGE. CRC fail. ");
					#endif
					break;
				}
				
				#ifdef DEBUG
				uart_println("CMD_PAGE. CRC OK");
				#endif
				
				if(Page.Part >= PACKETSPERPAGE || Page.PageNumber > (FLASHAPP * 1024 / (PAGESIZE * 2)))
				{
					USB_Serial_Putchar(2);
					#ifdef DEBUG
					uart_println("CMD_PAGE. Too far. ");
					#endif
					break;
				}
				
				// all seems ok, let's flash it
				
				for(u8 i = 0; i < 16; i++)
				{
					NVM_Load_Flash_Page_Buffer(Page.Part * 16 + i, Page.Payload[i * 2] | (Page.Payload[i * 2 + 1] << 8));
				}
				
				PageNumber = Page.PageNumber;
				
				USB_Serial_Putchar(1);
				
				#ifdef DEBUG
				uart_println("CMD_PAGE. OK. ");
				#endif
			break;
			
			case CMD_WRITE:
				PageNumber *= PAGESIZE;
				#ifdef DEBUG
				uart_send("CMD_WRITE. Writing at ");
				memdump(&PageNumber, 2);
				uart_println("");
				#endif
				NVM_Write_Flash_Page(PageNumber);
			break;
					
			case CMD_BLWRITE:
			case CMD_BLDNVM:
				while(USB_Serial_Pending() < 2);
				
				Magic = USB_Serial_Get() | USB_Serial_Get() << 8;
				
				if(Magic != BL_MAGIC)
				{
					USB_Serial_Putchar(3);
					break;
				}
				
				if(Command == CMD_BLDNVM) 
					NVM_Write_Flash_Page(PageNumber);
				else
				{ // CMD_BLWRITE
					USB_Detach();
					USB.CTRLA = 0;
					
					//SelfUpdate();
				}
			break;
			default:
				#ifdef DEBUG
				uart_println("Unknown.");
				#endif
			break;
		}
	}
	CRC_Init(CRC_RESET0_bm | CRC_SOURCE_IO_gc);
}

void FetchPage(CMD_PAGE_t *Page)
{
	byte *Address = (byte *)Page;
	for(int i = 0; i < sizeof(CMD_PAGE_t); i++)
	{
		while(USB_Serial_Pending() == 0);
		Address[i] = USB_Serial_Get();
	}
}

void PrepareForApplication()
{
	#ifndef DEBUG
	__asm__("jmp 0");
	#endif
}
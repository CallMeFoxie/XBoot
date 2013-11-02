/*
 * nvm.s
 *
 * Created: 29.12.2012 17:03:27
 *  Author: Ondra
 */ 

#include <avr/io.h>
#include "nvm.h"

/*
	Params:
		uint16_t address (R24:R25)
		uint8_t command (R23)
*/
.global NVM_Command
NVM_Command:
	MOV		ZL, R24
	MOV		ZH, R25
	STS		NVM_CMD, R22
	LPM		R24, Z
	RET

.global NVM_Erase_Flash_Page_Buffer
NVM_Erase_Flash_Page_Buffer:
	PUSH	R16

	LDI		R16, 0x26 ; NVM_CMD_ERASE_FLASH_BUFFER_gc
	STS		NVM_CMD, R22
	LDI		R16, 0xD8 ; CCP_IOREG_gc
	OUT		CCP, R16
	LDI		R16, NVM_CMDEX_bm
	STS		NVM_CTRLA, R16
NVM_Erase_Flash_Page_Buffer_Wait:
	LDS		R16, NVM_STATUS
	SBRC	R16, NVM_NVMBUSY_bp
	RJMP	NVM_Erase_Flash_Page_Buffer_Wait

	POP		R16
	RET

/*
	Params:
		u16: Page offset (R24:R25)
		u16: Data (R22:R23)
*/
.global NVM_Load_Flash_Page_Buffer
NVM_Load_Flash_Page_Buffer:
	PUSH	ZL
	PUSH	ZH

	MOV		ZL, R24 // "load Z pointer with the word address to write"
	MOV		ZH, R25
	LDI		R24, 0x23 ; NVM_CMD_LOAD_FLASH_BUFFER_gc // now can be used as temporary reg
	STS		NVM_CMD, R24
	MOV		R0, R22
	MOV		R1, R23
	LDI		R24, 0x9D ; CCP_SPM_gc
	OUT		CCP, R24
	
	SPM
	
	POP		ZH
	POP		ZL
	RET

/*
	Params:
		u16: Page (R24:R25)
*/
.global NVM_Erase_Flash_Page
NVM_Erase_Flash_Page:

	RET

/*
	Params:
		u32: Page (R22:R23:R24:R25)
*/
.global NVM_Write_Flash_Page
NVM_Write_Flash_Page:
	PUSH	ZL
	PUSH	ZH

	IN		ZL, RAMPZ
	PUSH	ZL

	OUT		RAMPZ, R23

	MOV		ZL, R24
	MOV		ZH, R25

	LDI		R24, 0x2E ; NVM_CMD_WRITE_FLASH_PAGE_gc
	LDI		R25, 0x9D ; CCP_SPM_gc

	OUT		CCP, R25
	STS		NVM_CMD, R24

	SPM

	POP		ZL
	OUT		RAMPZ, ZL

	POP		ZH
	POP		ZL
	RET

/*
	Params:
		u32: Start address (R22:R23:R24:R25)
		u32: Stop address (R18:R19:R20:R21)
*/
.global NVM_Flash_Range_CRC
NVM_Flash_Range_CRC:

	RET

/*
	Params:
		u32: Page number
*/
.global NVM_Erase_Write_Flash_Page
NVM_Erase_Write_App_Page:

	RET
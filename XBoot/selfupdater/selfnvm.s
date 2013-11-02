
/*
 * selfnvm.s
 *
 * Created: 13.1.2013 0:38:15
 *  Author: Ondra
 */ 

#include <avr/io.h>
.section .selfupdater,"ax"

/*
	Params:
		u8 Number of pages to copy (R25)
		u16 Page size (R23:R24)
		u32 Target address (R19:R20:R21:R22)
*/
.global SELFNVM_CopyPages
SELFNVM_CopyPages:
	PUSH	ZL
	PUSH	ZH
	PUSH	YL
	PUSH	YH
	PUSH	XL
	PUSH	XH
	//MOVW	R0, R24
	CLR		R1
	OUT		RAMPZ, R1 // rampz = 0
	OUT		RAMPY, R1 // rampy = 0
	OUT		RAMPX, R1

	CLR		ZH
	CLR		ZL // clear pointers - bootloader is saved on the beginning of the flash

	// now, for every page copy <pagesize> words to <target address>++
	//   RAMPZ:Z: target address for ELPM/SPM. First it loads <page size> bytes to RAM and then flashes whole page to avoid switching RAMPZ all the time
	//	 X: sub-page counter
	//	 Y: memory pointer (started from 0x2000 -- beginning of RAM. Ignore any heap, ignore everything, we will be rebooting after that anyway
	
SELFNVM_CopyPages_CopyPage:
	LDI		YL, lo8(0x2000) // Y = 0x2000 ... 0x2000 + PAGESIZE
	LDI		YH, hi8(0x2000)
	MOV		XH, R24
	MOV		XL, R23 // sub-page counter

SELFNVM_CopyPages_LoadByte:
	LPM		R16, Z+
	STD		Y+, R16

	SBIW	R26, 1 // memptr--;
	BRNE	SELFNVM_CopyPages_LoadByte 
	// now there should be whole page loaded in memory.

	LDI		YL, lo8(0x2000) // reset memory counter
	LDI		YH, hi8(0x2000)

SELFNVM_CopyPages_WriteByte:
	

	SUBI	R25, 1 // pages--;
	BRNE	SELFNVM_CopyPages_CopyPage // if (R25 != 0) SELFNVM_CopyPages_CopyPage
	
	POP		R1
	OUT		RAMPZ, R1
	POP		XH
	POP		XL
	POP		YH
	POP		YL
	POP		ZH
	POP		ZL
	CLR		R1
	RET
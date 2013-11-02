/*
 * board.h
 *
 * Created: 10.1.2013 19:24:30
 *  Author: Ondra
 */ 


#ifndef BOARD_H_
#define BOARD_H_

#include "../Conf.h"

#ifdef _XBOARD_ULTRA_
#include "ultra.h"
#endif

#ifdef _XBOARD_COCO_
#include "coco.h"
#endif

#ifdef _XBOARD_MINI_
#include "mini.h"
#endif

#define PAGES		(FLASHAPP + FLASHBL) * 1024 / (PAGESIZE * 2) // number of pages on the flash alltogether

#define PAGESAPP	FLASHAPP * 1024 / (PAGESIZE * 2) // number of pages in application flash
#define PAGESBL		FLASHBL * 1024 / (PAGESIZE * 2)  // number of pages in bootloader  flash

#define MEMEND		(MEMSIZE * 1024) - 1 + 0x2000
//#define STACKVAL	MEMEND - 32

#define TEMPBLPAGES	1024 / (PAGESIZE * 2) // number of pages reserved for temporary flashing bootloader for self-updating option. 1kB should be enough, 
										  // that leaves 7kB for the BL itself. Can be probably lowered down to 512B (we need just few lines of C/ASM code
										  // to run the few NVM commands)
										  
#define PACKETSPERPAGE	(PAGESIZE * 2) / 32

#define TEMPBLOFFSET	(FLASHAPP + FLASHBL - 1) * 1024

#endif /* BOARD_H_ */
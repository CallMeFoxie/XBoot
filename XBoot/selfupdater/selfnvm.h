/*
 * selfnvm.h
 *
 * Created: 13.1.2013 0:38:22
 *  Author: Ondra
 */ 


#ifndef SELFNVM_H_
#define SELFNVM_H_

#include "../drivers/global.h"
#include <avr/io.h>

#ifndef __ASSEMBLER__

extern void SELFNVM_CopyPages(u8 PagesCount, u16 PageSize, u32 TargetAddress) __attribute__ ((section (".selfupdater")));
 
#endif


#endif /* SELFNVM_H_ */
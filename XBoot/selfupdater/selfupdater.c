/*
 * Self_Updater.c
 *
 * Created: 13.1.2013 0:27:39
 *  Author: Ondra
 */ 

#include "selfupdater.h"

void SelfUpdate()
{
	USB.CTRLB = 0;
	USB.CTRLA = 0;
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = 0;
	
	/*for(u8 i = 0; i < PAGESBL - TEMPBLPAGES; i++)
	{ // for every page...
		for(u8 j = 0; j < PAGESIZE; j++)
		{ // for every page... LOAD from PAGESIZE * i + j <word> and save to the flash page buffer
			
		}
		
		// and write
	}*/
	
	SELFNVM_CopyPages(PAGESBL - TEMPBLPAGES, PAGESIZE, FLASHAPP * 1024 / 2);
}
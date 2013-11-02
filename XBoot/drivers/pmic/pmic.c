/*
 * pmic.c
 *
 * Created: 29.12.2012 23:04:27
 *  Author: Ondra
 */ 

#include "pmic.h"
#include <avr/interrupt.h>

void PMIC_Enable(byte Levels)
{
	if((PMIC.CTRL & PMIC_IVSEL_bm) && !(Levels & PMIC_IVSEL_bm))
	{
		CCP = CCP_IOREG_gc; // if IVSEL is set no more, we have to clear it (or all of it) first
		PMIC.CTRL = 0;
	}
	
	CCP = CCP_IOREG_gc;
	
	PMIC.CTRL = Levels/* | PMIC_IVSEL_bm*/;
	
	sei();
}
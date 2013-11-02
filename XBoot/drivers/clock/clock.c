/*
 * clock.c
 *
 * Created: 29.12.2012 17:04:20
 *  Author: Ondra
 */

#include "clock.h"
#include "../nvm/nvm.h"
#include "../global.h"
#include "../uart/uart.h"

void Clock_Set_USB(uint8_t Speed)
{
	OSC.CTRL = OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_RC32KEN_bm;

	while(!(OSC.STATUS & OSC_RC32KRDY_bm));
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));
	
	// tune DFLL2M to 2M, it will serve as base clock.
	DFLLRC2M.COMP1 = 0xA1;
	DFLLRC2M.COMP2 = 0x07;
	DFLLRC2M.CTRL = DFLL_ENABLE_bm;

	// tune DFLL32M to 48MHz, it will serve as USB clock
	DFLLRC32M.COMP1 = 0x80;
	DFLLRC32M.COMP2 = 0xBB;
	//DFLLRC32M.COMP1 = 0xB7;
	//DFLLRC32M.COMP2 = 0x1B;
	
	u16 CAL = NVM_Command(offsetof(NVM_PROD_SIGNATURES_t, USBRCOSC), NVM_CMD_READ_CALIB_ROW_gc) << 8;
	CAL |= NVM_Command(offsetof(NVM_PROD_SIGNATURES_t, USBRCOSCA), NVM_CMD_READ_CALIB_ROW_gc);
	
	DFLLRC32M.CALB = CAL >> 8;
	DFLLRC32M.CALA = CAL & 0xFF;
	
	//DFLLRC32M.CALA = 0x28;
	//DFLLRC32M.CALB = 0x28;
	//OSC.DFLLCTRL = OSC_RC32MCREF_USBSOF_gc;
	OSC.DFLLCTRL = OSC_RC32MCREF_RC32K_gc;
	
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;
	

	// prepare PLL for internal CPU source clock
	OSC.PLLCTRL = (Speed >> 1) | OSC_PLLSRC_RC2M_gc; // multiply by Speed / 2 to get desired clock, clock source = 2MHz RC
	OSC.CTRL |= OSC_PLLEN_bm;

	while(!(OSC.STATUS & OSC_PLLRDY_bm)); // wait for PLL to startup and catch

	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_PLL_gc; // CPU is now running at 32MHz from PLL

	CLK.USBCTRL = CLK_USBSRC_RC32M_gc | CLK_USBSEN_bm; // USB is running from the 32MHz @ 48MHz overclocked RC oscillator, tuned by USBSOF and DFLL
	
	#ifdef DEBUG
	uart_send("Set CAL to: ");
	memdump(&CAL, 2);
	uart_println("");
	#endif
}

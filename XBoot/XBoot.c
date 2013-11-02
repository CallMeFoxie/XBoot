/*
 * XBoot.c
 *
 * Created: 29.12.2012 17:00:22
 *  Author: Ondra
 */ 


#include <avr/io.h>
#include "drivers/global.h"
#include "drivers/uart/uart.h"
#include "drivers/usb/xboard/usb.h"
#include "drivers/usb/xboard/cdc/cdc.h"
#include "drivers/isp/isp.h"
#include "drivers/led/led.h"
#include "drivers/timer/timer.h"
#include <avr/interrupt.h>


volatile u8 OVFCounter = 0;

int main(void)
{	
	//PMIC.CTRL = PMIC_HILVLEN_bm | */PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	//sei();
	
	
	LED_Init();
	LED_On(Red);
	//LED_On(Orange);
	
	#ifdef DEBUG
    uart_init();
	LED_On(Orange);
	uart_println("UART ready.");
	LED_Off(Red);
	#endif
	
	USB_Init();
	#ifdef DEBUG
	uart_println("Attaching USB lines");
	#endif
	USB_Attach();
	
	#ifdef DEBUG
	uart_println("Init done. BL waiting...");
	#endif
	LED_Off(Red);
	LED_On(Orange);
	
	// wait for USB to come up within 7 secs. If it does not => jump to the application directly
	// if it does: check reset status. EXT = wait another 7 seconds in BL before jumping to the app itself for any programming stuff
	// if PO/WD/BOD = wait only 2 seconds before jumping to the app.
	
	char Keep = 1;
	
	OVFCounter = 0;
	
	TCD0.CTRLA = TC_CLKSEL_DIV64_gc; // 125ms or thereabouts
	TCD0.INTCTRLA = TC_OVFINTLVL_MED_gc;
	
	while(Keep) // wait for USB
	{
		if(OVFCounter >= 7*8) // 7 seconds
			Keep = 0; 
		if(USB_State == Configured)
			Keep = 0;
			
		if(OVFCounter & 1)
			LED_On(Red);
		else
			LED_Off(Red);
	}
	
	
	#ifdef DEBUG
	u8 DelayTime = 2*8; // 2 seconds just for the lulz. Will be set to 0 for production code.
	#else
	u8 DelayTime = 0;
	#endif
	
	Keep = 1;
	
	u8 rst = RST.STATUS;
	RST.STATUS = 0xFF; // clear the flags
	
	if(rst & RST_EXTRF_bm) // external reset -- stay in bootloader!
	{
		DelayTime = 7*8; // 7 seconds
	}
	
	if(USB_State == Configured)
	{
		LED_On(Red);
	}
	
	else if (rst & RST_PORF_bm) // power on -- jump to app
	{
		LED_Off(Orange);
		uart_println("DBG: Jumping to application.");
		
		//__asm__ ("jmp 0");
	}
	
	u8 GoToBL = 0;
	OVFCounter = 0;
	
	while(Keep)
	{
		if(OVFCounter >= DelayTime)
			Keep = 0;
			
		if(USB_Serial_Pending() > 0)
		{
			Keep = 0;
			GoToBL = 1;
		}
		
		if(OVFCounter & 1)
			LED_On(Orange);
		else
			LED_Off(Orange);
	}
	
	if(GoToBL && USB_State == Configured)
	{
		#ifdef DEBUG
		uart_println("Going to bootloader...");
		#endif
		
		LED_On(Orange);
		LED_On(Red);
			
		LaunchBootloader();
		
		while (1)
		{
		}
	}
	
	#ifdef DEBUG
	uart_println("Going to application...");
	#endif
		
	LED_Off(Orange);
	
	while(1)
	{
		
	}
}



ISR(TCD0_OVF_vect)
{
	OVFCounter++;
	//OVFCounter2++;
}
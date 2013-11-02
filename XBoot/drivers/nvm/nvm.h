/*
 * nvm.h
 *
 * Created: 29.12.2012 17:03:17
 *  Author: Ondra
 */ 


#ifndef NVM_H_
#define NVM_H_

#ifndef __ASSEMBLER__

#include "../global.h"

extern uint8_t NVM_Command(u16 Address, u16 Command);
extern void NVM_Erase_Flash_Page_Buffer();
extern void NVM_Load_Flash_Page_Buffer(u16 Offset, u16 Data);
extern void NVM_Erase_Flash_Page(u32 Page);
extern void NVM_Write_Flash_Page(u32 Page);
extern void NVM_Flash_Range_CRC(u32 Start, u32 Stop);
extern void NVM_Erase_Write_App_Page(u32 Page);

#endif

#endif /* NVM_H_ */
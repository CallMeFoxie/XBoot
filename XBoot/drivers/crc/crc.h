/*
 * crc.h
 *
 * Created: 12.1.2013 12:23:50
 *  Author: Ondra
 */ 


#ifndef CRC_H_
#define CRC_H_

#include "../global.h"
#include <avr/io.h>

void CRC_Init();
void CRC_Add(u8 Data);
u16 CRC_Result16();
u32 CRC_Result32();



#endif /* CRC_H_ */
/*
 * crc.c
 *
 * Created: 12.1.2013 12:23:55
 *  Author: Ondra
 */ 

#include "crc.h"

void CRC_Add(u8 Data)
{
	CRC.DATAIN = Data;
	//CRC.STATUS = CRC_BUSY_bm;
}

void CRC_Init()
{
	CRC.CTRL = CRC_RESET_RESET0_gc;
	CRC.CTRL = CRC_SOURCE_IO_gc;
	
}

u16 CRC_Result16()
{
	CRC.STATUS = CRC_BUSY_bm;
	while(CRC.STATUS & CRC_BUSY_bm);
	return CRC.CHECKSUM0 | (CRC.CHECKSUM1 << 8);
}

u32 CRC_Result32()
{
	return CRC.CHECKSUM0 | (CRC.CHECKSUM1 << 8) | (CRC.CHECKSUM2 << 16) | (CRC.CHECKSUM3 << 24);
}
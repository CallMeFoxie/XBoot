/*
 * global.h
 *
 * Created: 29.12.2012 17:05:37
 *  Author: Ondra
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <avr/io.h>

typedef unsigned char byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define LACR(addr, mask) *addr &= ~mask
#define LASR(addr, mask) *addr |= mask

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

#endif /* GLOBAL_H_ */
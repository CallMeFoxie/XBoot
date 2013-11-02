/*
 * ultra.h
 *
 * Created: 10.1.2013 19:22:14
 *  Author: Ondra
 */ 


#ifndef ULTRA_H_
#define ULTRA_H_

#ifndef BOARD_H_
#error "Include board/board.h instead!"
#endif

#define PAGESIZE	256 // pagesize in words
#define FLASHAPP	128 // size of app flash
#define FLASHBL		8 // size of BL flash


#define MEMSIZE		8 // memory size in kB

#define BOARDID		3

#endif /* ULTRA_H_ */
/*
 * mini.h
 *
 * Created: 10.1.2013 19:22:00
 *  Author: Ondra
 */ 


#ifndef MINI_H_
#define MINI_H_

#ifndef BOARD_H_
#error "Include board/board.h instead!"
#endif

#define PAGESIZE	128 // pagesize in words
#define FLASHAPP	128 // size of app flash
#define FLASHBL		8 // size of BL flash

#define MEMSIZE		8 // memory size in kB

#define BOARDID		1

#endif /* MINI_H_ */
/*
 * coco.h
 *
 * Created: 10.1.2013 19:22:07
 *  Author: Ondra
 */ 


#ifndef COCO_H_
#define COCO_H_

#ifndef BOARD_H_
#error "Include board/board.h instead!"
#endif

#define PAGESIZE	256 // pagesize in words
#define FLASHAPP	256 // size of app flash
#define FLASHBL		8 // size of BL flash


#define MEMSIZE		16 // memory size in kB

#define BOARDID		2

#endif /* COCO_H_ */
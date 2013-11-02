/*
 * Self_Updater.h
 *
 * Created: 13.1.2013 0:27:54
 *  Author: Ondra
 */ 


#ifndef SELF_UPDATER_H_
#define SELF_UPDATER_H_

#include <avr/io.h>
#include "../drivers/global.h"
#include "../board/board.h"
#include "selfnvm.h"


void SelfUpdate() __attribute__ ((section(".selfupdater")));


#endif /* SELF_UPDATER_H_ */
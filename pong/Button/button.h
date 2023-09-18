
#ifndef _BUTTON_H 
#define _BUTTON_H

#include "LPC17xx.h"
#include "../RIT/RIT.h"		  			

void BUTTON_init(void);

void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

#endif

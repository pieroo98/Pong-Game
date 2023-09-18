#include "button.h"

extern int down, down1,down2,flag,facile,medio,difficile,scelta;
extern char testo[100],testo2[20];

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	if(flag==2 || difficile==0){
		down2=1;	//reset game
		NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
		if(difficile!=2 && difficile!= -1){
			difficile=1;
			medio = facile= -1;
			scelta=0;
		}
		else if(scelta==1)
			facile=medio=difficile=0;
	}
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	if(flag==0 || facile==0){
		down = 1;		//start game
		NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
		if(facile!=2 && facile != -1){
			facile=1;
			medio = difficile = -1;
		}
	}
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	if(flag==1 || medio==0){
		down1=1;		//pause game
		NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
		if(medio!=2 && medio!=-1){
			medio=1;
			facile = difficile = -1;
		}
	}
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}



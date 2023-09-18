/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "adc/adc.h"
#include "Button/button.h"
//#include "LED/led.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

volatile char score[10] = {"0"},finish[10] = {"YOU LOSE"},vittoria[10]={"YOU WIN"},play_in_pause[10]={"PAUSE"},score2[10]={"0"},Reverse[12];
volatile char testo[100]={"key1 -> easy key2 -> medio"}, testo2[20]={"int0 -> hard"},endgame[20]={"press int0 to reset"},go[25]={"press key1 to continue"};

volatile unsigned short AD_current,AD_last = 120,pallina_X=182,pallina_Y=108; 
volatile short AD_curr2=182, AD_last2=182;
volatile int angle=0, primo=0, partenza=1,increment_y=0,increment_x=0, down=0, down1=0, down2=0,pause=0,flag=0;
volatile int facile=0,medio=0,difficile=0,scelta=0,increm=10;

//volatile const int freqs[2]={2120,1062};
int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	LCD_Clear(Black);
	BUTTON_init();
	init_RIT(0xF4240);									/* RIT Initialization 10 msec					*/
	enable_RIT();													/* RIT enabled												*/
	ADC_init();
	//LED_init();
	
	if(AD_last> 210)
			AD_last= 210;
	else if (AD_last<30)
		AD_last= 30;
	
	init_timer(0,0,0,3,0x61A8);			//1 ms ball
	init_timer(1,0,0,3,0x7A120);		//20 ms paddle2
	
	
	
//per provarlo su board fisica usare questi valori
	
//	init_RIT(0x989680);									/* RIT Initialization 100 msec					*/
//	init_timer(0,0,0,3,0x98968);		//0.025 s ball
//	init_timer(1,0,0,3,0x2625A0);		//0.1 s pad
	
// per il suono
//	init_timer(2,0,0,3,freqs[0]/8);	//suono
//	
//	LPC_PINCON->PINSEL1 |= (1<<21);       /* convertitore D/A			*/
//	LPC_PINCON->PINSEL1 &= ~(1<<20);			/* per il suono					*/
//	LPC_GPIO0->FIODIR |= (1<<26);					/*											*/

	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);				
	GUI_Text(7,145,(uint8_t *) testo, Yellow, Black,0);
	GUI_Text(7,180,(uint8_t *) testo2, Yellow, Black,0);
	start();
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

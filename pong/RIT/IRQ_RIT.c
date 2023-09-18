/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "RIT.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern unsigned short AD_current, AD_last, pallina_Y, pallina_X;
extern char score[10],finish[10],play_in_pause[10],score2[10],vittoria[10],Reverse[12],testo[100],testo2[20],endgame[20],go[25];
extern int angle,increment_y,increment_x, down, down1, down2,primo,flag,pause;
extern int facile,medio,difficile,scelta;

void RIT_IRQHandler (void)
{
	int var,var3;
	char tasto0[20]={"mod hard OK"}, tasto1[20]={"mod easy OK"}, tasto2[20]={"mod medio OK"},key1[20]={"press KEY1 to start"};
	
	
	if(down2!=0){
		if(((LPC_GPIO2->FIOPIN & (1<<10)) == 0) && (flag==2 && scelta==1 )){  //INT0
			down2++;
			switch(down2){
				case 2:
					GUI_Text(50, 180, (uint8_t *) endgame, Black, Black,0);
					var = atoi(score);
					var3 = atoi(score2);
					if(var==5 || var3 ==5){																
						if(var> var3){
							GUI_Text(90, 70, (uint8_t *) Reverse, Black, Black,1);
							GUI_Text(90, 260, (uint8_t *) vittoria, Black, Black,0);
						}
						else if( var3> var){
							GUI_Text(90, 260, (uint8_t *) finish, Black, Black,0);
							GUI_Text(90, 70, (uint8_t *) Reverse, Black, Black,1);
						}
					}
					GUI_Text(7,145,(uint8_t *) testo, Yellow, Black,0);
					GUI_Text(7,180,(uint8_t *) testo2, Yellow, Black,0);
					paint_ball(pallina_X,pallina_Y,Black);	
					var=var3=scelta=flag=0;
					sprintf(score,"%d",var);
					sprintf(score2,"%d",var3);
					start();
					break;
				default:
					break;
			}
		}
		else if(difficile==1){
			scelta=1;
			difficile =2;
			GUI_Text(7,145,(uint8_t *) testo, Black, Black,0);
			GUI_Text(7,180,(uint8_t *) testo2, Black, Black,0);
			GUI_Text(88,200,(uint8_t *) tasto0, Yellow, Black,0);
			GUI_Text(60,170,(uint8_t *) key1, Red, Black,0);
		}
		else {
			down2=0;
			NVIC_EnableIRQ(EINT0_IRQn);							 
			LPC_PINCON->PINSEL4    |= (1 << 20);     
		}
	}
	if(down!=0){
		if(((LPC_GPIO2->FIOPIN & (1<<11)) == 0) && (flag==0 && scelta==1)){  //KEY 1
			down++;
			switch(down){
				case 2:
					if(pause==0){
						paint_ball(pallina_X,pallina_Y,Black);
						change_paddle();
						pallina_X = 182;
						pallina_Y = 108;
						paint_ball(pallina_X,pallina_Y,Green);
						increment_x = increment_y = 1;
						angle = 45;
						enable_timer(2);
						NVIC_SetPriority(EINT2_IRQn, 1);
						NVIC_SetPriority(EINT1_IRQn, 2);
						GUI_Text(60,170,(uint8_t *) key1, Black, Black,0);
						var = atoi(score);
						var3 = atoi(score2);
						if(var>0 || var3 >0)
							GUI_Text(38,220,(uint8_t *) go, Black, Black,0);
						if(facile==2)
							GUI_Text(88,200,(uint8_t *) tasto1, Black, Black,0);
						else if(medio==2)
							GUI_Text(88,200,(uint8_t *) tasto2, Black, Black,0);
						else if(difficile==2)
							GUI_Text(88,200,(uint8_t *) tasto0, Black, Black,0);
					}
					else if (pause==1){	//riprendo il gioco
						pause =0;
						GUI_Text(100,160, (uint8_t *) play_in_pause, Black, Black,0);
						NVIC_SetPriority(EINT2_IRQn, 0);
						NVIC_SetPriority(EINT1_IRQn, 2);
						NVIC_SetPriority(EINT0_IRQn, 3);
					}
					enable_timer(1);
					enable_timer(0);
					primo = flag=1;
					break;
				default:
					break;
			}
		}
		else if(facile==1){
			scelta=1;
			facile=2;
			GUI_Text(7,145,(uint8_t *) testo, Black, Black,0);
			GUI_Text(7,180,(uint8_t *) testo2, Black, Black,0);
			GUI_Text(88,200,(uint8_t *) tasto1, Yellow, Black,0);
			GUI_Text(60,170,(uint8_t *) key1, Red, Black,0);
		}
		
		else {
			down=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 
			LPC_PINCON->PINSEL4    |= (1 << 22);     
		}
	}
	
	if(down1!=0){
		if(((LPC_GPIO2->FIOPIN & (1<<12)) == 0) && (flag==1 && scelta==1)){  //KEY 2
			down1++;
			switch(down1++){			//capire se vengo da un game over
				case 2:
					 if(pause==0){	//metto in pausa il gioco
						 pause = 1;
						 GUI_Text(100,160, (uint8_t *) play_in_pause, Yellow, Black,0);
						 disable_timer(0);
						 disable_timer(1);
						 primo=flag=0;
						 NVIC_SetPriority(EINT1_IRQn, 0);
						 NVIC_SetPriority(EINT2_IRQn, 2);
					 }
					break;
				default:
					break;
			}
		}
		else if (medio==1){
			scelta=1;
			medio =2;
			GUI_Text(7,145,(uint8_t *) testo, Black, Black,0);
			GUI_Text(7,180,(uint8_t *) testo2, Black, Black,0);
			GUI_Text(88,200,(uint8_t *) tasto2, Yellow, Black,0);
			GUI_Text(60,170,(uint8_t *) key1, Red, Black,0);
		}
		
		else {	
			down1=0;
			NVIC_EnableIRQ(EINT2_IRQn);							 
			LPC_PINCON->PINSEL4    |= (1 << 24);  
		}
	}		
  LPC_RIT->RICTRL |= 0x1;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

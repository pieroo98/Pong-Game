/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "timer.h"

extern unsigned short pallina_Y, pallina_X, AD_current;
extern int angle, primo,increment_y,increment_x,partenza,flag,AD_curr2;
extern char score[10],finish[10],score2[10],vittoria[10],Reverse[12],endgame[20],go[25];

//per il suono
//extern const int freqs[2];
//int ticks=0;
//uint16_t SinTable[45] =                                       
//{
//    410, 467, 523, 576, 627, 673, 714, 749, 778,
//    799, 813, 819, 817, 807, 789, 764, 732, 694, 
//    650, 602, 550, 495, 438, 381, 324, 270, 217,
//    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
//    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
//};

void TIMER0_IRQHandler (void)
{
	//per il suono
//	disable_timer(2);
//	reset_timer(2);
//	init_timer(2,0,0,3,freqs[0]/8);
//	enable_timer(2);

	static int tmp = 0,parete, fatto=0,pad ;
	int var,var2;
  if (LPC_TIM0->IR & 01)
	{
		if(primo>=1){
			fatto=1;
			pad = paddle();
			parete = muro();
			NVIC_SetPriority(TIMER0_IRQn, 0);
			
			if (pad==1 && partenza==0)	//pallina che colpisce paddle 1
				modifica_speed(AD_current);

			else if (pad ==4 ){							//giocatore 2
				modifica_speed(AD_curr2);
				increment_y += -2*increment_y;
			}
			
			else if(parete==1){				//muri
				tmp = increment_y;
				incremento_coord(&increment_x,&increment_y,180-angle);
				angle = 180 - angle;
				if((tmp>0 && increment_x>0) || (tmp>0 && increment_x<0))
					increment_y += 2*tmp;
			}
			else if(parete==0)		//inizio
				partenza = 0;
			else if( parete==5 || parete ==6){//game over
				primo=flag=0;
				var = atoi(score);
				var2 = atoi(score2);
				if(pallina_Y<50 && var<=5){
					var++;
					GUI_Text(10, 160, (uint8_t *) score, Black, Black,0);
					sprintf(score,"%d",var);
					GUI_Text(10, 160, (uint8_t *) score, White, Black,0);
					if(var>var2 && var ==5){																	
						flag=2;
						Reverse_word(finish);
						GUI_Text(90, 70, (uint8_t *) Reverse, White, Black,1);
						GUI_Text(90, 260, (uint8_t *) vittoria, White, Black,0);
					}
					else if( var2> var && var2==5){																
						flag=2;
						Reverse_word(vittoria);
						GUI_Text(90, 260, (uint8_t *) finish, White, Black,0);
						GUI_Text(90, 70, (uint8_t *) Reverse, White, Black,1);
					}
				}
				else if(pallina_Y>=START_PADDLE_Y0 && var2<=5){
					var2++;
					GUI_Text(230, 176, (uint8_t *) score2, Black, Black,1);
					sprintf(score2,"%d",var2);
					GUI_Text(230, 176, (uint8_t *) score2, White, Black,1);
					if( var2> var && var2==5){																
						flag=2;
						Reverse_word(vittoria);
						GUI_Text(90, 260, (uint8_t *) finish, White, Black,0);
						GUI_Text(90, 70, (uint8_t *) Reverse, White, Black,1);
					}
					else if(var>var2 && var ==5){																	
						flag=2;
						Reverse_word(finish);
						GUI_Text(90, 70, (uint8_t *) Reverse, White, Black,1);
						GUI_Text(90, 260, (uint8_t *) vittoria, White, Black,0);
					}
				}
				if(var==5 || var2==5){	//fine partita
					GUI_Text(50, 180, (uint8_t *) endgame, Red, Black,0);
				}
				else
					GUI_Text(38,220,(uint8_t *) go, Grey, Black,0);
				disable_timer(0);
				disable_timer(1);
				NVIC_SetPriority(EINT0_IRQn, 1);
				NVIC_SetPriority(EINT1_IRQn, 2);
			}
			if(pad==3 || pad==5)//colpisco il pad 1 o 2 lateralmente
				increment_x += -2*increment_x;
			
			else if(pad==2){	// se finisco in parte dentro il paddle_1 lo ridisegno appena sopra
				pallina_Y -= (pallina_Y +4) - (START_PADDLE_Y0 -1 );
				paint_ball(pallina_X,pallina_Y,Green);
				fatto=2;
			}
			else if(pad==6){
				pallina_Y = 42;
				paint_ball(pallina_X,pallina_Y,Green);
				fatto=2;
			}

			if( pallina_X -2<=20 && (pallina_Y+5>=160 && pallina_Y<=176))	//riscrittura score
					GUI_Text(10, 160, (uint8_t *) score, White, Black,0);
			if( pallina_X +2>=220 && (pallina_Y>=155 && pallina_Y<=176))	//riscrittura score2
					GUI_Text(230, 176, (uint8_t *) score2, White, Black,1);
		}
			//gestisco collisione bordo rosso - pallina
		if(primo>=1 && (((pallina_X>=227 && increment_x>0) || (increment_x<0 && pallina_X<=12)) || (pallina_Y<= 12 && increment_y < 0)))
			gestione_bordo(&fatto);
		
		//collisione pallina - paddle 1
		if((primo>=1 && ( pallina_Y +4+increment_y>=START_PADDLE_Y0-1 && increment_y>0)) && (pallina_X +2+increment_x>=AD_current-25 && pallina_X -2+increment_x< AD_current+25 ))
			gestione_pad_ball(&fatto);
		
		//collisione pallina - paddle 2
		if((primo>=1 && ( pallina_Y +increment_y<=42-1 && increment_y<0)) && (pallina_X +2+increment_x>=AD_curr2-25 && pallina_X -2+increment_x< AD_curr2+25 ))
			gestione_pad_ball_2(&fatto);

		if(fatto==1 && primo>=1)
			change_ball(&increment_x,&increment_y);
		
		LPC_TIM0->IR = 1;			
	}
	else if(LPC_TIM0->IR & 02)
  {
		LPC_TIM0->IR =  2 ;			
	}
	else if(LPC_TIM0->IR & 4)
  {
		LPC_TIM0->IR =  4 ;	
	}
	else if(LPC_TIM0->IR & 8)
  {
		LPC_TIM0->IR =  8 ;
	}
  return;
}

void TIMER1_IRQHandler (void)
{
  if (LPC_TIM1->IR & 01)
	{
		if(primo>=1)
			ADC_start_conversion();	//aggiorno il paddle ogni 20 ms
		LPC_TIM1->IR = 1;			
	}
	else if(LPC_TIM1->IR & 02)
  {
		LPC_TIM1->IR =  2 ;			
	}
	else if(LPC_TIM1->IR & 4)
  {
		LPC_TIM1->IR =  4 ;			
	}
	else if(LPC_TIM1->IR & 8)
  {
		LPC_TIM1->IR =  8 ;	
	}
  return;
}

void TIMER2_IRQHandler (void)
{
	if (LPC_TIM2->IR & 01)
	{
		//LPC_TIM2->TC; per prendere il valore del timer dal timer counter.
		//per il suono
//		static int flag=0;
//		LPC_DAC->DACR = SinTable[ticks]<<6;
//		ticks++;
//		flag++;
//		if(ticks==45) 
//			ticks=0;
//		if (flag==45){
//			disable_timer(2);
//			flag=0;
//		}	
		LPC_TIM2->IR = 1;			
	}
	else if(LPC_TIM2->IR & 02)
  {
		LPC_TIM2->IR =  2 ;	
	}
	else if(LPC_TIM2->IR & 4)
  {
		LPC_TIM2->IR =  4 ;
	}
	else if(LPC_TIM2->IR & 8)
  {
		LPC_TIM2->IR =  8 ;
	}
  return;
}

void TIMER3_IRQHandler (void)
{
	if (LPC_TIM3->IR & 01)
	{
		LPC_TIM3->IR = 1;	
	}
	else if(LPC_TIM3->IR & 02)
  {
		LPC_TIM3->IR =  2 ;
	}
	else if(LPC_TIM3->IR & 4)
  {
	 
		LPC_TIM3->IR =  4 ;
	}
	else if(LPC_TIM3->IR & 8)
  {
		LPC_TIM3->IR =  8 ;
	}
  return;
}

void paint_ball(int pallina_X, int pallina_Y, uint16_t Color){
	int j;
	for(j=0; j<5; j++)
			LCD_DrawLine(pallina_X -2, pallina_Y+j,pallina_X +2, pallina_Y+j,Color);
}

void modifica_speed(int AD){
	if((pallina_X>=AD +PADDLE_SX && pallina_X<= AD +PADDLE_SX+5) || (pallina_X>=AD +PADDLE_DX-5 && pallina_X<= AD + PADDLE_DX)){
		if(increment_x<0)
			angle = 180-22;
		else
			angle = 22;
	}
	else if((pallina_X>=AD +PADDLE_SX+6 && pallina_X<= AD +PADDLE_SX+11) || (pallina_X>=AD +PADDLE_DX-11 && pallina_X<= AD + PADDLE_DX-6)){
		if(increment_x<0)
			angle = 180-60;
		else
			angle = 60;
	}
	else if((pallina_X>=AD +PADDLE_SX+12 && pallina_X<= AD +PADDLE_SX+17) || (pallina_X>=AD +PADDLE_DX-17 && pallina_X<= AD + PADDLE_DX-12)){
		if(increment_x<0)
			angle = 180-70;
		else
			angle = 70;
	}
	else {
		if(increment_x<0)
			angle = 180-75;
		else
			angle = 75;
	}
	incremento_coord(&increment_x,&increment_y,angle);
}

void gestione_bordo(int *fatto){
	int var, var2;
	var=var2=0;
	if(pallina_X +2 +increment_x>=235 && increment_x>0){
		var = 234-(pallina_X+2);					
		if(pallina_Y + increment_y <=4 && increment_y<0)
			var2 = 5- pallina_Y;
		else 
			var2 = increment_y;
	}
	else if(pallina_X -2 +increment_x<=4 && increment_x<0){
		var = 5 -( pallina_X -2);
		if(pallina_Y + increment_y <=4 && increment_y<0)
			var2 = 5- pallina_Y;
		else 
			var2 = increment_y;
	}
	else if(pallina_Y+ increment_y <=4 && increment_y<0){
		var2 = 5- pallina_Y;
		if(pallina_X +2 +increment_x>=235 && increment_x>0)
			var = 234-(pallina_X+2);
		else if(pallina_X -2 +increment_x<=4 && increment_x<0)
			var = 5 -( pallina_X -2);
		else
			var = increment_x;
		}
	if(var!=0 || var2!=0){ 
		*fatto=2;
		change_ball(&var,&var2);
	}
}

void gestione_pad_ball(int *flag){
	int var=0;
	var = (START_PADDLE_Y0-1) - (pallina_Y +4);
	*flag =2;
	change_ball(&increment_x,&var);
}

void gestione_pad_ball_2(int *flag){
	int var=0;
	var = (42) - (pallina_Y);
	*flag =2;
	change_ball(&increment_x,&var);
}

extern void Reverse_word(char *stringa){
	int i, lun;
	lun = strlen(stringa);
	for(i=0;i<lun;i++)
		Reverse[i] = stringa[lun -i -1];
	Reverse[i]='\0';
}

/******************************************************************************
**                            End Of File
******************************************************************************/

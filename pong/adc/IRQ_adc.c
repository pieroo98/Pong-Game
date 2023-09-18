/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/
#include "adc.h"

extern unsigned short AD_current, AD_last;
extern int primo, increment_x,pallina_X,flag,facile,medio,difficile,increm;
extern short AD_curr2, AD_last2;

void ADC_IRQHandler(void) {	
	if(primo==1){
		paint_paddle(49,-49,AD_last2,32);
		change_paddle();
		change_paddle2();
	}
}

void change_paddle(void){
	int diff_AD;
	cattura_AD_current();
	diff_AD = (int)AD_current - (int)AD_last;
  if(diff_AD<-2 || diff_AD> 2){
		if(diff_AD > 0 && diff_AD<50){
			delate_paddle(diff_AD-50,-50,AD_last,START_PADDLE_Y0);
			paint_paddle(diff_AD,-49,AD_current,START_PADDLE_Y0);
		}
		else if(diff_AD > 0 && diff_AD>=50){
			delate_paddle(-1,-50,AD_last,START_PADDLE_Y0);
			paint_paddle(49,-49,AD_current,START_PADDLE_Y0);
		}
		else if(diff_AD<0 && diff_AD>-50){
			delate_paddle(-1,diff_AD,AD_last,START_PADDLE_Y0);
			paint_paddle(49,diff_AD,AD_current,START_PADDLE_Y0);
		}
		else{
			delate_paddle(-1,-50,AD_last,START_PADDLE_Y0);
			paint_paddle(49,-49,AD_current,START_PADDLE_Y0);
		}
		AD_last = AD_current;
  }
}

void cattura_AD_current(void){
	AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);
	AD_current = AD_current*240/0xfff;
	if(AD_current > 210)
			AD_current = 210;
	else if (AD_current<30)
		AD_current = 30;
}

void change_paddle2(void){
	int diff_AD;
	if(facile==2){
		AD_curr2 +=increm;
		if(AD_curr2> 210){
				AD_curr2= 210;
				increm=-7;
		}
		else if ((int)AD_curr2<30){
			AD_curr2= 30;
			increm=7;
		}
	}
	else if(medio==2){
		AD_curr2 = pallina_X;
		if(increment_x>0)
			AD_curr2 +=9;
		else 
			AD_curr2 =AD_curr2 - 9;
		if(AD_curr2> 210)
				AD_curr2= 210;
		else if ((int)AD_curr2<30)
			AD_curr2= 30;
	}
	else if(difficile==2){
		AD_curr2 = pallina_X;
		if(increment_x>0)
			AD_curr2 +=18;
		else 
			AD_curr2 =AD_curr2 - 18;
		if(AD_curr2> 210)
				AD_curr2= 210;
		else if ((int)AD_curr2<30)
			AD_curr2= 30;
	}
	diff_AD = (int)AD_curr2 - (int)AD_last2;
  if(diff_AD<=-1 || diff_AD>= 1){
		if(diff_AD > 0 && diff_AD<50){
			delate_paddle(diff_AD-50,-50,AD_last2,32);
			paint_paddle(diff_AD,-49,AD_curr2,32);
		}
		else if(diff_AD > 0 && diff_AD>=50){
			delate_paddle(-1,-50,AD_last2,32);
			paint_paddle(49,-49,AD_curr2,32);
		}
		else if(diff_AD<0 && diff_AD>-50){
			delate_paddle(-1,diff_AD,AD_last2,32);
			paint_paddle(49,diff_AD,AD_curr2,32);
		}
		else{
			delate_paddle(-1,-50,AD_last2,32);
			paint_paddle(49,-49,AD_curr2,32);
		}
		AD_last2 = AD_curr2;
  }
}

void delate_paddle(int diff_pos, int diff_neg,unsigned short POS_X, int POS_Y){
	int j;
	for(j=0; j<10; j++)
		LCD_DrawLine(POS_X-PADDLE_SX + diff_neg,POS_Y +j,POS_X - PADDLE_SX + diff_pos,POS_Y +j, Black);		
}
	
void paint_paddle(int diff_pos, int diff_neg, int pos_X, int POS_Y){
	int j;
	for(j=0;j<10;j++)
		LCD_DrawLine(pos_X+PADDLE_DX - diff_pos ,POS_Y+j,pos_X + PADDLE_SX - diff_neg,POS_Y +j,Green);
}

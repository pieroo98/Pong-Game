#ifndef _ADC_H_
#define _ADC_H_

#include "lpc17xx.h"
#include "../GLCD/GLCD.h"
#include <time.h>
#include <stdlib.h>

#define PADDLE_SX -25
#define PADDLE_DX 24
#define START_PADDLE_Y0 278

/* lib_adc.c */
extern void ADC_init (void);
extern void ADC_start_conversion (void);

/* IRQ_adc.c */
extern void ADC_IRQHandler(void);
extern void delate_paddle(int diff_pos, int diff_neg,unsigned short AD_last, int y);
extern void paint_paddle(int diff_pos, int diff_neg, int pos,int y);
extern void change_paddle(void);
extern void cattura_AD_current(void);
extern void change_paddle2(void);
#endif

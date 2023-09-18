																			 /*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_led, funct_led .c files
** Correlated files:    lib_led.c, funct_led.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef _LED_H
#define _LED_H
#include "lpc17xx.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define N 8

/* lib_led */
void LED_init(void);
void LED_deinit(void);

/* funct_led */
void LED_On (unsigned int num);
void LED_Off (unsigned int num);
void LED_Out(unsigned int value);
void LED_Out_reverse(unsigned int value);
void LED_Out_signed(signed int value);
void LED_Out_compl_a_2(int8_t value);
char *bin_to_Exa(int *bin);
int bin_to_dec(int *bin);
int exa_to_dec(char* exa);
int *dec_to_bin(int dec);
char *dec_to_exa(int dec);
int *exa_to_bin(char * exa);

#endif

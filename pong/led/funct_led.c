/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_led.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        High level led management functions
** Correlated files:    lib_led.c, funct_led.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "led.h"

#define LED_NUM     8                   /* Number of user LEDs                */
const unsigned long led_mask[] = { 1UL<<0, 1UL<<1, 1UL<<2, 1UL<< 3, 1UL<< 4, 1UL<< 5, 1UL<< 6, 1UL<< 7 };

extern unsigned char led_value;

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On(unsigned int num) {
 
  LPC_GPIO2->FIOPIN |= led_mask[num];
	led_value = LPC_GPIO2->FIOPIN;
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off(unsigned int num) {

  LPC_GPIO2->FIOPIN &= ~led_mask[num];
	led_value = LPC_GPIO2->FIOPIN;
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
	led_value = value;
}

void LED_Out_reverse(unsigned int value) {
  int i,len,d,v[N]={0};
	uint8_t reverso;
	reverso=value%2;
	d=value/2;
	v[0]=reverso;
	//converto in binario
	for (i=1;i<N && d!=0;i++)
	{
			reverso=d%2;
			d=d/2;
			v[i]=reverso;
	}
	//ritorno in decimale con bit opposti
	for (i=N-1, reverso=0, len=0;i>=0;i--)
			reverso=reverso + v[i]*(int)(pow(2,len++));
	value = reverso;
  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
	led_value = value;
}

void LED_Out_signed(signed int value){
	int i;
  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
	led_value = value;
}

void LED_Out_compl_a_2(int8_t value){
	int i;
	if(value<=-1){
		value--;
		value = value ^ 0xff;
		value = value | 0x80;
	}
	for (i = 0; i < LED_NUM; i++) {
		if (value & (1<<i)) {
			LED_On (i);
		} else {
			LED_Off(i);
		}
	}
	led_value = value;
}

char *bin_to_Exa(int *bin){
	//verificare la dimensione del vettore bin se effettivamente di 32
	int x=0,i,a=0,y=0,c,count,j;
  char *vettExa,tmp[2];
	vettExa = (char *)malloc(sizeof(char)*(32/4));
	for(i=0, a=0 ;i<c ; i=i+4, a++)
	{
		for(j=i+3, count=0, x=0, y=0; j>=0 && count!=4; j--, count++, y++)
				x=x + bin[j]*(int)(pow(2,y));
		if(x==10)
				vettExa[a]='A';
		else if(x==11)
				vettExa[a]='B';
		else if(x==12)
				vettExa[a]='C';
		else if(x==13)
				vettExa[a]='D';
		else if(x==14)
				vettExa[a]='E';
		else if(x==15)
				vettExa[a]='F';
		else{
				sprintf(tmp,"%d",x);
				vettExa[a]=tmp[0];
		}
	}
	return vettExa;
}

int bin_to_dec(int *bin){
	//verificare se su 32 bit
	int i,x=0,y=0;
	for (i=32;i>=0;i--)
        x=x + bin[i]*(int)(pow(2,y++));
	return x;
}

int exa_to_dec(char* exa){
	int i,x=0,y=0,tmp2=0;
	//verificare se su 32 bit, quindi 8 caratteri
	for (i=32/4 - 1, x=0, y=0; i>=0; i--, y++){
		if(isdigit(exa[i]))
			tmp2 = exa[i];
		else{
			if(exa[i] =='a' || exa[i]=='A')
				tmp2 = 10;
			else if(exa[i] =='b' || exa[i]=='B')
				tmp2 = 11;
			else if(exa[i] =='c' || exa[i]=='C')
				tmp2 = 12;
			else if(exa[i] =='d' || exa[i]=='D')
				tmp2 = 13;
			else if(exa[i] =='e' || exa[i]=='E')
				tmp2 = 14;
			else if(exa[i] =='f' || exa[i]=='F')
				tmp2 = 15;
		}
		x=x + tmp2*(int)(pow(16,y));
	}
	return x;
}

int *dec_to_bin(int dec){
	int *v,i,d,r;
	v = (int *)calloc(sizeof(int),32);
	r=dec%2;
	d=dec/2;
	//salvo i bit partendo dalla posizione 31 e salendo fino a 0, così sono gia ordinati i bit
	v[31]=r;
	for (i=30;i>=0 && d!=0;i--)
	{
			r=d%2;
			d=d/2;
			v[i]=r;
	}
	return v;
}

char *dec_to_exa(int dec){
	char *exa;
	int *v;
	v = (int *)malloc(sizeof(int)*32);
	v = dec_to_bin(dec);
	exa = (char *)malloc(sizeof(char)*8);
	exa = bin_to_Exa(v);
	return exa;
}

int *exa_to_bin(char * exa){
	int dec = exa_to_dec(exa);
	int *v;
	v=(int *) malloc(sizeof(int)*32);
	v=dec_to_bin(dec);
	return v;
}

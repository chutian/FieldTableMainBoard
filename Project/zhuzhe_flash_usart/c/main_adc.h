#ifndef _MAIN_ADC
#define _MAIN_ADC

#include "stm32f10x.h"

 #define THS 256

#define MIN_ADC     350
#define MAX_ADC     700
//#define MAX_ADC     

void main_adc_init(void);
uint16_t main_adc_getdata(char channel);
char  get_adc_key(char key);
char get_light_adc(char key);
extern uint16_t pre_adc[4];




#endif

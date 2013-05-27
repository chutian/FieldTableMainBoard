#ifndef _SYS_FUNC
#define _SYS_FUNC
#include "stm32f10x.h"	
//#include "lcd128.h"
#include "screen_uart.h"
#include "main_adc.h"
#include "audio.h"

#include "music.h"	   
#include "usart_flash.h"
//--------------------
 
//#include "MP3Sample.c"
#include "vs1003.h"
//----------------------

#define OK_LENGTH 3708
//#define UP_LENGTH 4772
#define XIAOHUI_LENGTH 7570
#define MUSIC_LENGTH 5468
#define UP_LENGTH 1968
#define DOWN_LENGTH 1962
#define LEFT_LENGTH 1844
#define RIGHT_LENGTH 1998
#define SQUARE_LENGTH 1402	
//extern char arrowdown[1962+8],arrowleft[1844+8],arrowright[1998+8],arrowup[1968+8],square[1402+8];

void RCC_Configuration(void);
void sys_main(void);

//extern unsigned char MP3_DATA[];
#endif

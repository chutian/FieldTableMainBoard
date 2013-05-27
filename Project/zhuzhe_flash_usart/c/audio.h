#ifndef _AUDIO_H
#define _AUDIO_H
#include "stm32f10x.h"
#include "vs1003.h"
#include "music.h"
#include "usart_flash.h"
#define AUDIO_SIZE 0x2000
#define AUDIO_ADDR 250

void stop_music(void);
void start_music(char mode);
void play_music(void);

extern unsigned char audio_data[AUDIO_SIZE];

#endif

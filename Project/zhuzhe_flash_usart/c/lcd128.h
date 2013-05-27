#ifndef _lcd_128
#define _lcd_128

// move to h file
#include "stm32f10x.h"
#define DISP_BACK_LIGHT   0X15
#define DISP_HIGH_SPEED   0X01
#define	DISP_RESET        0X01
#define DISP_KEY_ACK      0X09
#define DISP_FULL_COLOR   0x22
#define DISP_DOWNLOAD     0X32
#define DISP_RECALL       0X33
#define FULL_SIZE   	1
//(128*128*2)
#define BIT_SW      0X80
#define BIT_ONLINE  0X08
#define BIT_CMDOK   0X04
#define BIT_NACK    0X02
#define BIT_BUSY    0X01

//else than 0~2
#define SCREEN_STAT_IDLE          0x10
#define SCREEN_STAT_CMDOK         0X40
#define SCREEN_STAT_ERR_ONLINE    0x80
#define SCREEN_STAT_ERR_NACK      0x20
#define SCREEN_STAT_ERR_MISSED    0x08

//----------------------------------------------
#define NAME_MAX     7
#define NAME_MIN     1
#define KEY_NUM      (NAME_MAX-NAME_MIN+1)


#define SCREEN_SS6       GPIO_Pin_8 

#define SCREEN_PIN_SS1       GPIO_Pin_3 
#define SCREEN_PIN_SS2       GPIO_Pin_4 
#define SCREEN_PIN_SS3       GPIO_Pin_2 
#define SCREEN_PIN_SS4       GPIO_Pin_0 
#define SCREEN_PIN_SS5       GPIO_Pin_8 
#define SCREEN_PIN_SS6       GPIO_Pin_8 
#define SCREEN_PIN_SS7       GPIO_Pin_9 

#define SCREEN_PIN_SS8       GPIO_Pin_8 
#define SCREEN_PIN_SS9       GPIO_Pin_8 

#define SCREEN_PORT_SS1  GPIOE
#define SCREEN_PORT_SS2  GPIOE
#define SCREEN_PORT_SS3  GPIOE
#define SCREEN_PORT_SS4  GPIOB
#define SCREEN_PORT_SS5  GPIOA
#define SCREEN_PORT_SS6  GPIOD
#define SCREEN_PORT_SS7  GPIOB 

#define SCREEN_PORT_SS8  GPIOD
#define SCREEN_PORT_SS9  GPIOD

#define SCREEN_MISO	 GPIO_Pin_14
#define SCREEN_MOSI	 GPIO_Pin_15
#define SCREEN_SCLK  GPIO_Pin_13
#define SCREEN_PORT GPIOB
//-----------------------------------------------------
void screen_init(void);
void screen_update(char name,char *pdata);
void screen_spi_isp(void);
void screen_key_ack(char name); 
void screen_down(char dispID,char name,char *pdata);
void screen_recall(char name,char dispID);
void screen_highspeed(char name,char *pdata);
void screen_down_rle(char name,char dispID, char *pdata,unsigned int data_length);

extern struct disp_data *screen_pcmd;
extern char   key_value,key_flag,screen_init_stat[KEY_NUM];
extern char   screen_head[20],screen_data[FULL_SIZE],screen_tailer[10];

#endif

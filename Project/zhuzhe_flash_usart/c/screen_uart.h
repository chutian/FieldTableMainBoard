#ifndef _SCREEN_UART
#define _SCREEN_UART




#include "stm32f10x.h"


//move to header
#define UART_UP   USART1
#define UART_DOWN USART2
#define BUFF_SIZE 200
#define PACKAGE_SIZE
#define HEADER_SIZE	 2
#define DATA_SIZE    4
#define FILE_SIZE    0x2000

#define PACKAGE_BUFF_SIZE	200
extern char package_buff0[PACKAGE_BUFF_SIZE],package_in,package_out;

#define BACKLIGHT 0X27
#define BLACKWHITE 0X02
struct screen_uart_datatype
{
   USART_TypeDef *  uart;  
  char send_buf[BUFF_SIZE];
  uint16_t index_in;
  uint16_t index_out;
  uint16_t flag_busy;
  uint16_t number;
};
//---------------------------------------------

void screen_uart_init(void);
void screen_uart_send(char usart,char data);
void screen_uart_up_isp(void);
void screen_uart_down_isp(void);
void screen_uart_main_down_isp(void);
void screen_uart_tosend(char send_data);
void screen_uart_package(char uart,char *data);

void screen_uart_sends(char usart,char  *pdata,char len);

extern char flag_cmd,cmd_data[DATA_SIZE],stat,file_data[FILE_SIZE],flag_tosend,data_tosend;

extern struct screen_uart_datatype screen_uart_var[2];

extern uint16_t uart_pic_length;
#endif

#ifndef __USART_FLASH_H
#define __USART_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f10x.h"
#include "usart_flash.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_flash.h"
#include "misc.h"
#include "fsmc_nand.h"
#include "fsmc_nand.h"
#include "stm32_eval.h"
#include <stdio.h>
#include "usart_flash.h"
/* Private typedef -----------------------------------------------------------*/
/*神舟III号LED灯相关定义*/
#define RCC_GPIO_LED                    RCC_APB2Periph_GPIOF    /*LED使用的GPIO时钟*/
#define LEDn                            4                       /*神舟III号LED数量*/
#define GPIO_LED                        GPIOF                   /*神舟III号LED灯使用的GPIO组*/

#define DS1_PIN                         GPIO_Pin_6              /*DS1使用的GPIO管脚*/
#define DS2_PIN                         GPIO_Pin_7				/*DS2使用的GPIO管脚*/
#define DS3_PIN                         GPIO_Pin_8  			/*DS3使用的GPIO管脚*/
#define DS4_PIN                         GPIO_Pin_9				/*DS4使用的GPIO管脚*/
/* Private define ------------------------------------------------------------*/
void Uart_Configuration(void);
void ReadDate(uint8_t *date,unsigned long lenth,unsigned int DateNumber);
void WriteDate(uint8_t *date,unsigned long lenth,unsigned  int DateNumber);
void UpdateAddress(unsigned char *address,NAND_ADDRESS  NewAddress,unsigned int DateNumber);
void GetAddress(unsigned char *address);



#ifdef __cplusplus
}
#endif

#endif 

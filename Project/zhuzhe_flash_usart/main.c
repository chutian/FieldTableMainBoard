#include "sys.h"
#include "fsmc_nand1.h"
#include "stm32_eval.h"
#include <stdio.h>
#include "usart_flash.h"
#define BUFFER_SIZE         0x2000

NAND_IDTypeDef NAND_ID;
GPIO_InitTypeDef GPIO_InitStructure;
NAND_ADDRESS WriteReadAddr;
USART_InitTypeDef USART_InitStructure;

static uint8_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];
__IO uint32_t PageNumber = 4, WriteReadStatus = 0, status= 0;
volatile uint32_t j = 0;

/* Private function prototypes -----------------------------------------------*/
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);

//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */

int main(void)
{
    uint16_t k=0,i=0; 
   for(k=0;k<30;k++) for(i=0;i<60000;i++);
	 Uart_Configuration();
//  /*将写Nand Flash的数据BUFFER填充为从0x25开始的连续递增的一串数据 */
//     Fill_Buffer(TxBuffer,BUFFER_SIZE,0x10);
//	 WriteDate(TxBuffer,BUFFER_SIZE,2);
//	 ReadDate (RxBuffer,BUFFER_SIZE,2);
//    /*判断读回的数据与写入的数据是否一致*/  
//    for(j = 0; j < BUFFER_SIZE; j++)
//    {
//      if(TxBuffer[j] != RxBuffer[j])
//      {
//        WriteReadStatus++;
//      }
//    }
//
//
//    if (WriteReadStatus == 0)
//    { 
//  	  j=1;		 //0k!!!
//    }
//    else
//    { 
//   	  j=0;	  	  
//	  
//    }
	sys_main();
   while(1)
  {
   }

}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */


void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}
//
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//  USART_SendData(EVAL_COM1, (uint8_t) ch);
//
//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
//  {
//  }
//
//  return ch;
//}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/**
  * @}
  */ 

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

#include "usart_flash.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_flash.h"
#include "misc.h"
#include "fsmc_nand1.h"
#include "stm32_eval.h"
#include <stdio.h>
#include "usart_flash.h"
ErrorStatus HSEStartUpStatus;
unsigned char RxData;
unsigned char TxData;
__IO NAND_ADDRESS  NewAddress2;
__IO NAND_ADDRESS  table1;
__IO NAND_ADDRESS  NewAddress1;
__IO NAND_ADDRESS  table4;
__IO uint32_t page =0;
__IO uint32_t page1 =0;
__IO static uint8_t table2 [0x800];
__IO static uint8_t table3 [0x800];
__IO static uint8_t TxDate [0x1800],RxDate [0x1800];
unsigned char nowaddress [3];
/*
*********************************************************************************************************
*	函 数 名: Uart_Configuration
*	功能说明: 初始化串口设备
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
   	RCC_DeInit();//将RCC外设寄存器设置为缺省值
	RCC_HSEConfig(RCC_HSE_ON);//设置外部晶振
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //使能预取缓冲区
    	FLASH_SetLatency(FLASH_Latency_2); 	//Flash2个等待周期   
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHB时钟 = 系统时钟
		RCC_PCLK1Config(RCC_HCLK_Div2);//APB1时钟 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div1);//APB2时钟 = HCLK
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//设置PLL时钟源为外部的8M，倍频9倍即为72M	
		RCC_PLLCmd(ENABLE);//使能PLL时钟

		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);//等待PLL准备好
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);// 选择PLL做为系统时钟源    
		while(RCC_GetSYSCLKSource() != 0x08);//
	}
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//使能GPIOA时钟
  
    
    /* Enable the USARTy Interrupt */
   	 /*使能FSMC时钟 */
 
//   #ifdef  VECT_TAB_RAM
//    /* Set the Vector Table base location at 0x20000000 */
//   NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
//   #else  /* VECT_TAB_FLASH  */
   /* Set the Vector Table base location at 0x08000000 */
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//   #endif		
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	NAND_Init();
}


 /*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 串口中断处理函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
// void USART1_IRQHandler(void)
//{
//   
//	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		RxData = USART_ReceiveData(USART1);//接收8位的数据
//	    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){};
//	}
//		//溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题
//	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
//	{
//		USART_ClearFlag(USART1,USART_FLAG_ORE); //读SR其实就是清除标志
//		USART_ReceiveData(USART1); //读DR	
//	}  
//} 
 
 /*
*********************************************************************************************************
*	函 数 名: GetAddress
*	功能说明: 取得文件索引列表
*	形    参：unsigned char *address 数组元素务必小于0X400!!!!!  uint16_t *New_Address
*	返 回 值: 
*********************************************************************************************************
*/
void GetAddress(unsigned char *address)
{
 table4.Zone = 0x00;
 table4.Block = 0x00;
 table4.Page = 0x00;
 NAND_ReadSmallPage (address, table4, 1);
} 
/*
*********************************************************************************************************
*	函 数 名: UpdateAddress
*	功能说明: 更新文件索引列表
*	形    参：unsigned char *address 数组元素务必小于0X400!!!!!	NAND_ADDRESS  NewAddress; 新的地址 unsigned int DateNumber 数据标号
*	返 回 值: 无
*********************************************************************************************************
*/
 void UpdateAddress(unsigned char *address,NAND_ADDRESS  NewAddress,unsigned int DateNumber)
{
 
 table1.Zone = 0x00;
 table1.Block = 0x00;
 table1.Page = 0x00;
 NAND_EraseBlock(table1);

 address[DateNumber*3] =   NewAddress2.Zone;
 address[DateNumber*3+1] = NewAddress2.Block;
 address[DateNumber*3+2] = NewAddress2.Page;

 NAND_WriteSmallPage (address,table1,1);

}

/*
*********************************************************************************************************
*	函 数 名: WriteDate
*	功能说明: 写入数据
*	形    参：date 写入的数据数组 lenth 数组的长度 DateNumber 数据标号 
*	返 回 值: 无
*********************************************************************************************************
*/
 void WriteDate(uint8_t *date,unsigned long lenth,unsigned  int DateNumber)
{
 
 
 unsigned char i;
 GetAddress(table2);
 if((table2[1] == 0xff )&& (table2[2] == 0xff))
 {
 NewAddress2.Zone = 0x00;
 NewAddress2.Block = 0x01;
 NewAddress2.Page = 0x00;
 table2[0] = 0x00;
 table2[1] = 0x01;
 table2[2] = 0x00;
 }
 else
 {
 if(table2[DateNumber*3+1] != 0xff)		//重复利用
 {
  NewAddress2.Zone = table2[DateNumber*3];
  NewAddress2.Block =table2[DateNumber*3+1];
  NewAddress2.Page = table2[DateNumber*3+2];
 }
 else
 {
  NewAddress2.Zone = table2[0];
  NewAddress2.Block =(table2[1]+1);
  NewAddress2.Page = table2[2];
  table2[1]++;
 }
 }
  UpdateAddress(table2,NewAddress2,DateNumber);
  GetAddress(table2);
  NAND_EraseBlock(NewAddress2);
  NAND_WriteSmallPage(date,NewAddress2,4);
}

/*
*********************************************************************************************************
*	函 数 名: ReadDate
*	功能说明: 读取数据
*	形    参：date 读出的数据数组 lenth 数组的长度 DateNumber 数据标号
*	返 回 值: 无
*********************************************************************************************************
*/
 void ReadDate(uint8_t *date,unsigned long lenth,unsigned int DateNumber)
{
 GetAddress(table3);
 NewAddress1.Zone = table3 [DateNumber*3];
 NewAddress1.Block = (table3 [DateNumber*3+1]);
 NewAddress1.Page = table3 [DateNumber*3+2];
 NAND_ReadSmallPage (date,NewAddress1,4);
}  
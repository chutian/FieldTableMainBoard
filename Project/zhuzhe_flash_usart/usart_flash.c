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
*	�� �� ��: Uart_Configuration
*	����˵��: ��ʼ�������豸
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
   	RCC_DeInit();//��RCC����Ĵ�������Ϊȱʡֵ
	RCC_HSEConfig(RCC_HSE_ON);//�����ⲿ����
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //ʹ��Ԥȡ������
    	FLASH_SetLatency(FLASH_Latency_2); 	//Flash2���ȴ�����   
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHBʱ�� = ϵͳʱ��
		RCC_PCLK1Config(RCC_HCLK_Div2);//APB1ʱ�� = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div1);//APB2ʱ�� = HCLK
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//����PLLʱ��ԴΪ�ⲿ��8M����Ƶ9����Ϊ72M	
		RCC_PLLCmd(ENABLE);//ʹ��PLLʱ��

		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);//�ȴ�PLL׼����
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);// ѡ��PLL��Ϊϵͳʱ��Դ    
		while(RCC_GetSYSCLKSource() != 0x08);//
	}
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOAʱ��
  
    
    /* Enable the USARTy Interrupt */
   	 /*ʹ��FSMCʱ�� */
 
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
*	�� �� ��: USART1_IRQHandler
*	����˵��: �����жϴ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
// void USART1_IRQHandler(void)
//{
//   
//	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		RxData = USART_ReceiveData(USART1);//����8λ������
//	    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){};
//	}
//		//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����
//	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
//	{
//		USART_ClearFlag(USART1,USART_FLAG_ORE); //��SR��ʵ���������־
//		USART_ReceiveData(USART1); //��DR	
//	}  
//} 
 
 /*
*********************************************************************************************************
*	�� �� ��: GetAddress
*	����˵��: ȡ���ļ������б�
*	��    �Σ�unsigned char *address ����Ԫ�����С��0X400!!!!!  uint16_t *New_Address
*	�� �� ֵ: 
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
*	�� �� ��: UpdateAddress
*	����˵��: �����ļ������б�
*	��    �Σ�unsigned char *address ����Ԫ�����С��0X400!!!!!	NAND_ADDRESS  NewAddress; �µĵ�ַ unsigned int DateNumber ���ݱ��
*	�� �� ֵ: ��
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
*	�� �� ��: WriteDate
*	����˵��: д������
*	��    �Σ�date д����������� lenth ����ĳ��� DateNumber ���ݱ�� 
*	�� �� ֵ: ��
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
 if(table2[DateNumber*3+1] != 0xff)		//�ظ�����
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
*	�� �� ��: ReadDate
*	����˵��: ��ȡ����
*	��    �Σ�date �������������� lenth ����ĳ��� DateNumber ���ݱ��
*	�� �� ֵ: ��
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
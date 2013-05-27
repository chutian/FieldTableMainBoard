//main_adc
#include "main_adc.h"
#include "vs1003.h"

//-------------------------------
//move to header

//--------------------


ADC_InitTypeDef ADC_InitStructure;
//DMA_InitTypeDef DMA_InitStructure;
 uint16_t pre_adc[4]={0};
 
char  get_adc_key(char key)
{
  uint16_t temp=0,temp2=0,temp3=0;
       if(key>3) return 0;
	   temp=main_adc_getdata(key);
	   temp2=pre_adc[key]+THS;
	   if(pre_adc[key]<THS) temp3=0;
	   else                 temp3=pre_adc[key]-THS;
	   if((temp>temp2)||(temp<temp3))
	   {
	   	 
	     pre_adc[key]=temp;
	   	 return 1;
	   }
	   else                              return 0;
}		    

char get_light_adc(char key)
{
    uint16_t temp=0,temp1=0,temp2=0;
    temp=main_adc_getdata(key);
    delay_ms(100);
    temp1=main_adc_getdata(key);
    delay_ms(100);
    temp2=main_adc_getdata(key);
    if((temp<MIN_ADC)&&(temp1<MIN_ADC)&&(temp2<MIN_ADC))
    {
      //pre_adc[key]=1;  
			return 1;
    }
    if((temp>MAX_ADC)&&(temp1>MAX_ADC)&&(temp2>MAX_ADC))
    {
        //pre_adc[key]=2;
				return 2;
    }
    if(((temp>MIN_ADC)||(temp<MAX_ADC))&&((temp1>MIN_ADC)||(temp1<MAX_ADC))&&((temp2>MIN_ADC)||(temp2<MAX_ADC)))
    {
        //pre_adc[key]=3;
				return 3;
    }
    return 0;
}

uint16_t main_adc_getdata(char channel)
{
  volatile  uint16_t temp=100,temp2=223;
  //  /* Start ADC1 Software Conversion */ 
  
//     /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); 
  while(temp--);
  switch(channel)
  {
      case 0:   ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);break;
      case 1:   ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);break;
      case 2:   ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);break;
      case 3:   ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_55Cycles5);break;
	  default: return 12345;

  }	
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  temp=100;
  while(temp--);
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET); //检查制定ADC标志位置1与否 ADC_FLAG_EOC 转换结束标志位
 //while(ADC_GetSoftwareStartConvStatus(ADC1)==SET);
 temp2=ADC_GetConversionValue( ADC1 );
   ADC_SoftwareStartConvCmd(ADC1, DISABLE);
   
  ADC_Cmd(ADC1, DISABLE);
 return  temp2;


}


void main_adc_init()
{
     GPIO_InitTypeDef GPIO_InitStructure;  
	 volatile uint16_t dly=0;
  /* ADCCLK = PCLK2/2 */
  RCC_ADCCLKConfig( RCC_PCLK2_Div8); 

  /* Enable DMA1 clock */
 // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

  /* Configure Pc.04 (ADC Channel14) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode =DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode =DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  
     /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 DMA */
//  ADC_DMACmd(ADC1, ENABLE);
  	for(dly=10000;dly>0;dly--);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
//     
//  /* Start ADC1 Software Conversion */ 
//  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//临界代码&&超时


#include "sys.h"
//#include "MP3Sample.c"



void write(char id,uint16_t length,char* pdata){}

//uint16_t pic_length[5]={UP_LENGTH,DOWN_LENGTH,LEFT_LENGTH,RIGHT_LENGTH,SQUARE_LENGTH};  
//extern  char ok[3708+8],up[4772+8],xiaohui[7570+8],music[5468+8];
//char * pic_addr[5]={(char *)arrowup+8,(char *)arrowdown+8,(char *)arrowleft+8,(char *)arrowright+8,(char *)square+8};
static uint16_t j=0;
//char key_id[KEY_NUM]={0};
char blackwhite=0,key_stop=0,flag_set_light=0,flag_down=0;

void sys_main(void)
{		   
   volatile uint16_t i,k=0,last;
   char packet_buff[7],temp=0;
   char tempADC,tempADC1;
    
   for(k=0;k<30;k++) for(i=0;i<60000;i++);
//	  RCC_Configuration(); 
   screen_uart_init();
   main_adc_init();
/* 
//语音芯片控制部分 
//      VS1003_Init();
//   VS1003_Reset();
//   VS1003_SoftReset();

*/
   while(1)
   {
	 if(flag_down==0)
	 {	 
            __disable_irq();
	   	  while(flag_tosend)   //临界代码
		  {
		    
			 flag_tosend--;
                         temp=package_buff0[package_out];		   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
			 package_out++;	  
			 if(package_out>PACKAGE_BUFF_SIZE)
			 {
			       package_out-=PACKAGE_BUFF_SIZE;
			 }

		    screen_uart_tosend(temp);
			//flag_tosend=0;
		  }	
            __enable_irq();
	   	  if((get_adc_key(0)||get_adc_key(1)))
		  {
		    flag_set_light=1;
// 				if(pre_adc[0]<0x600)
// 				{
// 					pre_adc[0]=0x00;
// 				}
		    packet_buff[0]=0;packet_buff[1]=BACKLIGHT;packet_buff[2]=pre_adc[0]/256;packet_buff[3]=0xff;
	//		flag_tosend_light=1;
   //         screen_uart_package(1,packet_buff);
	//		screen_uart_package(1,packet_buff);
		  }
//此处为黑夜白天判断，此种方式判断是否会出现问题，需要考虑一下，也要考虑一下IO没有接上拉或下拉电阻也未接限流电阻是否会有把IO口烧掉的风风险。
	   	  if(get_adc_key(2))
		  {	 
                     packet_buff[0]=0;packet_buff[1]=BLACKWHITE;packet_buff[2]=0x0;
                     if((pre_adc[2]>3000)&&(blackwhite==0))	   //blackwhile=0=>white
                     {
                        blackwhite=1;packet_buff[3]=1;  
                        screen_uart_package(0,packet_buff);
                     }
                     else if((pre_adc[2]<1000)&&(blackwhite==1)) 
                     {					 
                        blackwhite=0;packet_buff[3]=0; 
                        screen_uart_package(0,packet_buff);
                     }
		  }	
/*
//因不需要单量调整个所以注释掉此部分		  
	   	  if(get_adc_key(3))
		  {	 
		    packet_buff[0]=0x0ff;packet_buff[1]=0x0ff;packet_buff[2]=0x0f9;packet_buff[3]=0x0a0;
		    if((pre_adc[3]>3000)&&(key_stop==0))	  
			{
			  key_stop=1;  
			}
			else if((pre_adc[3]<1000)&&(key_stop==1)) 
			{					 
			   key_stop=0; 	   
			   stop_music();
               screen_uart_package(0,packet_buff);
			}
		  }
*/	  
//		   if((screen_uart_var[1].flag_busy==0)&&(flag_set_light==1))
__disable_irq();
		   if((screen_uart_var[1].flag_busy==0)&&(stat==0)&&(flag_set_light==1))
		   {
		   	 flag_set_light=0;
			 packet_buff[0]=0x5a;packet_buff[1]=0x5a;
// 				 if(pre_adc[0]<0x600)
// 				{
// 					pre_adc[0]=0x00;packet_buff[5]=0x00;
// 				}
// 				else
// 				{
// 					packet_buff[5]=0xff;
// 				}
             if((pre_adc[0]>last+0x220)||(pre_adc[0]<last-0x220))
             {
                 last=pre_adc[0];
                 packet_buff[2]=0;packet_buff[3]=BACKLIGHT;packet_buff[4]=(pre_adc[0]/256);packet_buff[5]=0xff;packet_buff[6]=0xa5;
 //           screen_uart_package(1,packet_buff);
            screen_uart_sends(1,packet_buff,7);
             }
            
		   }
__enable_irq();
       }
		  if(flag_cmd)
		  {
		    packet_buff[0]=cmd_data[1];
		    packet_buff[1]=cmd_data[3]; 
//		    packet_buff[2]=cmd_data[2];  
		  	flag_cmd=0;
            switch(packet_buff[0])
			{
			   case 0x3:				  //图片刷新反馈
                                 packet_buff[0]=0x0;packet_buff[1]=0x4;packet_buff[2]=0x0;packet_buff[3]=0x01;
                                 screen_uart_package(0,packet_buff);
			         break;
			   case 0x0fa:						 //!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
//因不需要单量调整个所以注释掉此部分	
//			          if(packet_buff[0]==0x0fa)
//					  {
//					  	start_music(packet_buff[1]);
//					  } 
*/ 
				  break;
			   case 0x2:
			         if(packet_buff[1]==0x02)
                                 {
                                       packet_buff[0]=0x0;packet_buff[1]=0x2;packet_buff[2]=0x0;packet_buff[3]=blackwhite;
                                       screen_uart_package(0,packet_buff);
                                 }
			         break;
			   case 0x010: 
/* 
//因不需要单量调整个所以注释掉此部分	
                               flag_down=1;
			       if( (packet_buff[1]>=AUDIO_ADDR)&&(packet_buff[1]<=AUDIO_ADDR+4))
				   {
				   
                        __disable_irq();
 				        WriteDate(file_data,AUDIO_SIZE,packet_buff[1]);
                       __disable_irq();
                                   }
*/
                   break;
			  default: break;
						
			}
			flag_cmd=0;		  	
		  }
/*
   		 for(i=0;i<200;i++)
		 {
		   for(k=0;k<30;k++)
		   play_music();
		 }
 */  
   }

}

/*配置系统时钟,使能各外设时钟*/
void RCC_Configuration(void)
        {
           ErrorStatus  HSEStartUpStatus;

     RCC_DeInit();                     //RCC 系统复位
     RCC_HSEConfig(RCC_HSE_ON);      //开启HSE
     HSEStartUpStatus = RCC_WaitForHSEStartUp();  //等待HSE准备好
     if(HSEStartUpStatus == SUCCESS)
     {
       FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //Enable Prefetch Buffer
       FLASH_SetLatency(FLASH_Latency_2);      //Set 2 Latency cycles
       RCC_HCLKConfig(RCC_SYSCLK_Div1);   //AHB clock  = SYSCLK
       RCC_PCLK2Config(RCC_HCLK_Div1);   //APB2 clock = HCLK
       RCC_PCLK1Config(RCC_HCLK_Div2);   //APB1 clock = HCLK/2
       RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLLCLK = 12MHz * 6 = 72 MHz
       RCC_PLLCmd(ENABLE);          //Enable PLL
       while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  //Wait till PLL is ready  
       RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //Select PLL as system clock source
       while(RCC_GetSYSCLKSource() != 0x08);   //Wait till PLL is used as system clock source

        }
  }

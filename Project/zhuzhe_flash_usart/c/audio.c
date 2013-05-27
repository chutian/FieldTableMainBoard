#include "audio.h"

//#include "MP3Sample.c"

unsigned char audio_data[AUDIO_SIZE],flag_play=0;
uint16_t audio_mode,audio_index=0,audio_size=0;
void stop_music()
{
   flag_play=0;
}
void start_music(char mode)
{
  switch(mode)
  {
     case 0x0a1:audio_mode=0;break;
     case 0x0a2:audio_mode=1;break;
     case 0x0a3:audio_mode=2;break;
     case 0x0a4:audio_mode=3;break;
     case 0x0a5:audio_mode=4;break;
	 default: return;
  
  }
//    for(i=0;i<AUDIO_SIZE;i++)
//	{
//	  audio_data[i]=MP3_DATA[i+40000];
//	}  
   ReadDate (audio_data,AUDIO_SIZE,audio_mode+250); 
	  audio_size=(audio_data[0]<<8)|(audio_data[1]); 

	  audio_index=0;
	  flag_play=1;
//	   audio_size=music_len[audio_mode];

}
void play_music()
{
//	   flag_play=1;
//	   audio_mode=2;
//	   audio_size=music_len[audio_mode];
	   if(flag_play)
	   {
	      if(MP3_DREQ ==1)
		  {
		        
		        VS1003_WriteData( (void*)( &audio_data[audio_index+2]) ); 
	  
//		        VS1003_WriteData( (void*)( music_index[audio_mode] + audio_index) ); 
				audio_index += 32;
				if( audio_index >= audio_size	)
				{
				  audio_index-=audio_size;
				}
		 }
	   }  
 	
}


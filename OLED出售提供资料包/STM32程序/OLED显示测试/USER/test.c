#include <stm32f10x_lib.h>
#include "sys.h"	
#include "delay.h"	
#include "oled.h" 	 

//OLED显示 实验
//信德电子 XD Electronics
//网址:http://xdguo.taobao.com/
   
int main(void)
{		
 	u8 t=0;	    	
 	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
												   
	OLED_Init();			//初始化液晶      
	// 显示信德电子
	OLED_ShowHanziChar(32,0,0,1);
	OLED_ShowHanziChar(48,0,1,1);
	OLED_ShowHanziChar(64,0,2,1);
	OLED_ShowHanziChar(80,0,3,1);
 	OLED_ShowString(0,16," XD Electronics");
  OLED_ShowString(0,32,"     http://");
 	OLED_ShowString(0,48,"xdguo.taobao.com");  
	OLED_Refresh_Gram();
	while(1) 
	{		
		delay_ms(300);
	}	  
}


























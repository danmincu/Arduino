#include <stm32f10x_lib.h>
#include "sys.h"	
#include "delay.h"	
#include "oled.h" 	 

//OLED��ʾ ʵ��
//�ŵµ��� XD Electronics
//��ַ:http://xdguo.taobao.com/
   
int main(void)
{		
 	u8 t=0;	    	
 	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
												   
	OLED_Init();			//��ʼ��Һ��      
	// ��ʾ�ŵµ���
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


























#include "led.h"
	   

//初始化PE5为输出口.并使能时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED0-->PE.12 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE.12
 GPIO_SetBits(GPIOE,GPIO_Pin_12);						 //PE.12 输出高


}
 

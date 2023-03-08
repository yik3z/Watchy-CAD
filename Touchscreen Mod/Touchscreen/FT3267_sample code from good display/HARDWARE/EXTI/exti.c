#include "exti.h"
#include "delay.h"
#include "FT6336.h"
#include "i2c.h"
#include "led.h"
#include "usart.h"
#define Is_INT_IN  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) //PE.14
//外部中断初始化函数
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

	//GPIOE.14  中断线以及中断初始化配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource14);

	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	EXTI_ClearITPendingBit(EXTI_Line14);    //清除LINE14上的中断标志位
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;	//使能TOUCH_INT所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	
  
}
/*
判断外部中断状态以及清除外部状态标志位的函数 EXTI_GetFlagStatus 和 EXTI_ClearFlag，
在 EXTI_GetITStatus 函数中会先判断这种中断是否使能，使能了才去判断中断标志位，而
EXTI_GetFlagStatus 直接用来判断状态标志位。
*/
void EXTI15_10_IRQHandler(void)		//中断名在这个文件里startup_stm32f10x_hd.s 
{	
	delay_us(5);   //消抖(调节触摸响应速度)	
	if(Is_INT_IN == 0)	   	
	{	 
		 TPR_Structure.TouchSta |= TP_COORD_UD;				//触摸坐标有更新
		 EXTI_ClearITPendingBit(EXTI_Line14);//清除中断线14标志位

  }
}


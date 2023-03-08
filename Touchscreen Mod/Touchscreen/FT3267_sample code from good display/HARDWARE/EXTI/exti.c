#include "exti.h"
#include "delay.h"
#include "FT6336.h"
#include "i2c.h"
#include "led.h"
#include "usart.h"
#define Is_INT_IN  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) //PE.14
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��

	//GPIOE.14  �ж����Լ��жϳ�ʼ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource14);

	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	EXTI_ClearITPendingBit(EXTI_Line14);    //���LINE14�ϵ��жϱ�־λ
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;	//ʹ��TOUCH_INT���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	
  
}
/*
�ж��ⲿ�ж�״̬�Լ�����ⲿ״̬��־λ�ĺ��� EXTI_GetFlagStatus �� EXTI_ClearFlag��
�� EXTI_GetITStatus �����л����ж������ж��Ƿ�ʹ�ܣ�ʹ���˲�ȥ�ж��жϱ�־λ����
EXTI_GetFlagStatus ֱ�������ж�״̬��־λ��
*/
void EXTI15_10_IRQHandler(void)		//�ж���������ļ���startup_stm32f10x_hd.s 
{	
	delay_us(5);   //����(���ڴ�����Ӧ�ٶ�)	
	if(Is_INT_IN == 0)	   	
	{	 
		 TPR_Structure.TouchSta |= TP_COORD_UD;				//���������и���
		 EXTI_ClearITPendingBit(EXTI_Line14);//����ж���14��־λ

  }
}


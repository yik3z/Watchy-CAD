#include "led.h"
	   

//��ʼ��PE5Ϊ�����.��ʹ��ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED0-->PE.12 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOE.12
 GPIO_SetBits(GPIOE,GPIO_Pin_12);						 //PE.12 �����


}
 

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "i2c.h"
#include "FT6336.h"


/************************************************
FT6336UоƬ��֧�����㴥�أ��������5������
************************************************/
    
 int main(void)
 {
	u8 i=0;		 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
	EXTIX_Init();
	FT6336_Init();	
	LED0=0;					//�ȵ������ 
	while(1)
	{	

				if(TPR_Structure.TouchSta &TP_COORD_UD)		//�����а���
		{
			TPR_Structure.TouchSta=0;	//����
			FT6336_Scan();					 //��ȡ��������

			switch(touch_count)
			{
					case 1:
							if((TPR_Structure.x[0]!=0)&&(TPR_Structure.y[0]!=0)
								&&(TPR_Structure.x[0]<200)&&(TPR_Structure.y[0]<200))//����˵���Ч����
							{
							 printf("�����������:%d\r\n",touch_count);	//FT6336U���֧�����㴥��
							 printf("x0:%d,y0:%d\r\n",TPR_Structure.x[0],TPR_Structure.y[0]);
							}
							break;
					case 2:
							if((TPR_Structure.x[0]!=0)&&(TPR_Structure.y[0]!=0)
								&&(TPR_Structure.x[1]!=0)&&(TPR_Structure.y[1]!=0)
								&&(TPR_Structure.x[0]<200)&&(TPR_Structure.y[0]<200)
								&&(TPR_Structure.x[1]<200)&&(TPR_Structure.y[1]<200))//����˵���Ч����
							{
							 printf("�����������:%d\r\n",touch_count);	//FT6336U���֧�����㴥��
							 printf("x0:%d,y0:%d\r\n",TPR_Structure.x[0],TPR_Structure.y[0]);
							 printf("x1:%d,y1:%d\r\n",TPR_Structure.x[1],TPR_Structure.y[1]);
							}
							break;
					
					default:
						break;
						
					}
			//��ձ��
				for(i=0;i<2;i++)
				{
					TPR_Structure.x[i]=0;
					TPR_Structure.y[i]=0;
				}
							
			}
	}
	 
}

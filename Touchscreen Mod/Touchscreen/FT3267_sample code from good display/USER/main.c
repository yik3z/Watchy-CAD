#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "i2c.h"
#include "FT6336.h"


/************************************************
FT6336U芯片仅支持两点触控，驱动最大5寸屏。
************************************************/
    
 int main(void)
 {
	u8 i=0;		 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口 
	EXTIX_Init();
	FT6336_Init();	
	LED0=0;					//先点亮红灯 
	while(1)
	{	

				if(TPR_Structure.TouchSta &TP_COORD_UD)		//触摸有按下
		{
			TPR_Structure.TouchSta=0;	//清标记
			FT6336_Scan();					 //读取触摸坐标

			switch(touch_count)
			{
					case 1:
							if((TPR_Structure.x[0]!=0)&&(TPR_Structure.y[0]!=0)
								&&(TPR_Structure.x[0]<200)&&(TPR_Structure.y[0]<200))//软件滤掉无效操作
							{
							 printf("触摸点个数：:%d\r\n",touch_count);	//FT6336U最多支持两点触控
							 printf("x0:%d,y0:%d\r\n",TPR_Structure.x[0],TPR_Structure.y[0]);
							}
							break;
					case 2:
							if((TPR_Structure.x[0]!=0)&&(TPR_Structure.y[0]!=0)
								&&(TPR_Structure.x[1]!=0)&&(TPR_Structure.y[1]!=0)
								&&(TPR_Structure.x[0]<200)&&(TPR_Structure.y[0]<200)
								&&(TPR_Structure.x[1]<200)&&(TPR_Structure.y[1]<200))//软件滤掉无效操作
							{
							 printf("触摸点个数：:%d\r\n",touch_count);	//FT6336U最多支持两点触控
							 printf("x0:%d,y0:%d\r\n",TPR_Structure.x[0],TPR_Structure.y[0]);
							 printf("x1:%d,y1:%d\r\n",TPR_Structure.x[1],TPR_Structure.y[1]);
							}
							break;
					
					default:
						break;
						
					}
			//清空标记
				for(i=0;i<2;i++)
				{
					TPR_Structure.x[i]=0;
					TPR_Structure.y[i]=0;
				}
							
			}
	}
	 
}

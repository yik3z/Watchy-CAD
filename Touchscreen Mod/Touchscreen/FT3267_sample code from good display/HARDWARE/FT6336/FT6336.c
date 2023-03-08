#include "FT6336.h"
#include "delay.h"
#include "i2c.h"
#include "usart.h"



//-----------------------------------测试寻找地址-----------------------------------------------

/*
u16 check_addr;
u16 check_ok_times;
void i2c_write_byte_check(unsigned char txd)
{			   	
    u8 t;   
	SDA_OUT(); 	    
    i2c_scl=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        i2c_sda=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(10);   //对TEA5767这三个延时都是必须的
		i2c_scl=1;
		delay_us(10); 
		i2c_scl=0;	
		delay_us(10);
    }	      
	i2c_sda=1;delay_us(10);
	SDA_IN();	   
	i2c_scl=1;delay_us(10);			 
//	while(i2c_sda_rd);
	for(t=0;t<100;t++)//延时100us等待
		if(i2c_sda_rd)
			delay_us(1);
		else
			break;
	i2c_scl=0;
	
	//----------------------------搜索成功完成提示--------------------
	if(t < 100)
	{
		//string_normal(DIS_COL_BOUNDARY+1,UP_START+1,Red,"I2C_OK!");
		LCD_ShowString(260,310,400,16,24,"I2C_OK"); 
		check_ok_times++;
		//dis_int_5ad(250,20+check_ok_times*20,check_addr);
	}
}

void find_ic_addr(void)
{
	check_ok_times = 0;
	for(check_addr=0;check_addr<128;check_addr++)
	{
		i2c_start();
		//i2c_write_byte_check((check_addr<<1)|I2C_WR);
		i2c_write_byte_check((check_addr<<1));
		i2c_write_byte_check(0xA6);
		i2c_end();
	}
}
*/
//---------------------------------------------------------------------------------------------------




u8 FT6336_read_firmware_id(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_FIRMARE_ID);
}

//	u8 dat=0;
//
//	i2c_start();
//	i2c_write_byte((I2C_ADDR_FT6336<<1)|I2C_WR);
//	i2c_write_byte(0xA6);
//	i2c_end();
//
//	i2c_start();
//	i2c_write_byte((I2C_ADDR_FT6336<<1)|I2C_RE);
//	dat=i2c_read_byte();
//	i2c_end();
//	return(dat);

u8 FT6336_read_device_mode(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_DEVICE_MODE);
}

u8 FT6336_read_td_status(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TD_STATUS);
}

//第一触点
u8 FT6336_read_touch1_event(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_EVENT)>>4;
}

u8 FT6336_read_touch1_id(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_ID)>>4;
}

//左下角为坐标零点,水平Y轴,垂直X轴
//Y轴范围0~459
//X轴范围0~319
//touch1和touch2排列顺序为按面积排列,而ID号为按下的时间先后标号,该标号可代表按下的手指
//在第一点按下后ID记为0,事件为8,而抬起后ID与坐标保持最后数值,事件为4,此时第二个点数据一直为FF
//在第一点按下前提下按第二个点,ID记为1,事件为8,抬起后全恢复FF,而当第二个点的接触面积大于第一个点时,
//它将保持ID与坐标前提下与第一个点更换存储地址,第一个点抬起后,第二个点的依旧ID为1
//按下第三个点时,将与其它两个点进行面积对比后,直接抛弃面积最小的点
u16 FT6336_read_touch1_x(void)
{
	u8 read_buf[2];
	i2c_read_addr_str(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_X,2,read_buf);
	return ((read_buf[0]&0x0f)<<8)|read_buf[1];
}

u16 FT6336_read_touch1_y(void)
{
	u8 read_buf[2];
	i2c_read_addr_str(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_Y,2,read_buf);
	return ((read_buf[0]&0x0f)<<8)|read_buf[1];
}

//第二触点
u8 FT6336_read_touch2_event(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH2_EVENT)>>4;
}

u8 FT6336_read_touch2_id(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH2_ID)>>4;
}

u16 FT6336_read_touch2_x(void)
{
	u8 read_buf[2];
	i2c_read_addr_str(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH2_X,2,read_buf);
	return ((read_buf[0]&0x0f)<<8)|read_buf[1];
}

u16 FT6336_read_touch2_y(void)
{
	u8 read_buf[2];
	i2c_read_addr_str(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH2_Y,2,read_buf);
	return ((read_buf[0]&0x0f)<<8)|read_buf[1];
}


const u16 FT6236_TPX_TBL[5]=
{
	FT_TP1_REG,
	FT_TP2_REG,
	FT_TP3_REG,
	FT_TP4_REG,
	FT_TP5_REG
};

//坐标示意（FPC朝下）
////y轴////////    //264x176
							//
							//
							//x轴
							//
							//
u8 touch_count=0;
TouchPointRefTypeDef TPR_Structure; 
void FT6336_Scan(void)
{
	u8 i=0;
	u8 sta = 0;
	u8 buf[4] = {0};  
	u8 gestid=0;
	FT6336_RD_Reg(0x02,&sta,1);//读取触摸点的状态  
  touch_count=sta;	
	FT6336_RD_Reg(0x01,&gestid,1);//读取触摸点的状态
	
 	if(sta & 0x0f)	//判断是否有触摸点按下，0x02寄存器的低4位表示有效触点个数
 	{
 		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));	//~(0xFF << (sta & 0x0F))将点的个数转换为触摸点按下有效标志
 		for(i=0;i<2;i++)	                                //分别判断触摸点1-5是否被按下
 		{
 			if(TPR_Structure.TouchSta & (1<<i))			    //读取触摸点坐标
 			{											    //被按下则读取对应触摸点坐标数据
 				FT6336_RD_Reg(FT6236_TPX_TBL[i],buf,4);	//读取XY坐标值
				TPR_Structure.x[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
				TPR_Structure.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
 				if((buf[0]&0XC0)!=0X80)
 				{
					TPR_Structure.x[i]=TPR_Structure.y[i]=0;//必须是contact事件，才认为有效	
					TPR_Structure.TouchSta &=0xe0;	//清除触摸点有效标记
					return;
				}
 			}
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;     //触摸按下标记
 	}
 	else
 	{
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN) 	//之前是被按下的
 			TPR_Structure.TouchSta &= ~0x80;        //触摸松开标记	
 		else
 		{
 			TPR_Structure.x[0] = 0;
 			TPR_Structure.y[0] = 0;
 			TPR_Structure.TouchSta &=0xe0;	//清除触摸点有效标记
 		}
 	}
}


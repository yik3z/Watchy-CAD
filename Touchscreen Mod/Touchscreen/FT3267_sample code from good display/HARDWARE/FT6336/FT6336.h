#ifndef __FT6336_H
#define __FT6336_H	 


#include "stm32f10x.h"
#include "i2c.h"


#define LED_EN PBout(12)
extern u8 touch_count;//触摸点数标志位
void find_ic_addr(void);
void i2c_write_byte_check(unsigned char txd);
void FT6336_init(void);
u8 FT6336_read_firmware_id(void);
u8 FT6336_read_device_mode(void);
u8 FT6336_read_td_status(void);
u16 FT6336_read_touch1_x(void);
u16 FT6336_read_touch1_y(void);
u8 FT6336_read_touch1_event(void);
u8 FT6336_read_touch1_id(void);

u8 FT6336_read_touch2_event(void);
u8 FT6336_read_touch2_id(void);
u16 FT6336_read_touch2_x(void);
u16 FT6336_read_touch2_y(void);
void FT6336_Scan(void);



#define TP_PRES_DOWN 0x80  //触屏被按下	
#define TP_COORD_UD  0x40  //触摸坐标更新标记

//触摸点相关数据结构体定义
typedef struct			
{
	u8 TouchSta;	//触摸情况，b7:按下1/松开0; b6:0没有按键按下/1有按键按下;bit5:保留；bit4-bit0触摸点按下有效标志，有效为1，分别对应触摸点5-1；
	u16 x[5];		//支持5点触摸，需要使用5组坐标存储触摸点数据
	u16 y[5];
	
}TouchPointRefTypeDef;
extern TouchPointRefTypeDef TPR_Structure;

#define I2C_ADDR_FT6336 0x38

//FT6236 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT6236模式控制寄存器
#define FT_REG_NUM_FINGER       0x02		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//版本		
#define FT_ID_G_MODE 			0xA4   		//FT6236中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器  
#define Chip_Vendor_ID          0xA3        //芯片ID(0x36)
#define ID_G_FT6236ID			0xA8		//0x11


#define FT6336_ADDR_DEVICE_MODE 	0x00
#define FT6336_ADDR_TD_STATUS 		0x02
#define FT6336_ADDR_TOUCH1_EVENT 	0x03
#define FT6336_ADDR_TOUCH1_ID 		0x05
#define FT6336_ADDR_TOUCH1_X 		0x03
#define FT6336_ADDR_TOUCH1_Y 		0x05

#define FT6336_ADDR_TOUCH2_EVENT 	0x09
#define FT6336_ADDR_TOUCH2_ID 		0x0B
#define FT6336_ADDR_TOUCH2_X 		0x09
#define FT6336_ADDR_TOUCH2_Y 		0x0B

#define FT6336_ADDR_FIRMARE_ID 		0xA6
#endif

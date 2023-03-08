#ifndef __I2C_H
#define __I2C_H 	

#include "stm32f10x.h"

//I2C总线函数

//I2C读写命令	
#define FT_CMD_WR 	  0X70    	//写命令
#define FT_CMD_RD 		0X71		//读命

//IO口定义
#define FT6336_SDA_H() 	GPIO_SetBits(GPIOD,GPIO_Pin_10)
#define FT6336_SDA_L() 	GPIO_ResetBits(GPIOD,GPIO_Pin_10)

#define FT6336_SCL_H() 	GPIO_SetBits(GPIOD,GPIO_Pin_9)
#define FT6336_SCL_L() 	GPIO_ResetBits(GPIOD,GPIO_Pin_9)

#define FT6336_RST_H()	GPIO_SetBits(GPIOE,GPIO_Pin_15)
#define FT6336_RST_L()	GPIO_ResetBits(GPIOE,GPIO_Pin_15)

#define SDA_IN()  {GPIOD->CRH&=0X0FFFFFFF;GPIOD->CRH|=0X00000400;}	 //输入模式，浮空输入模式
#define SDA_OUT() {GPIOD->CRH&=0X0FFFFFFF;GPIOD->CRH|=0X00000100;}	 //通用推挽输出，输出速度50MHZ

#define Is_SDA_IN  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10) //PD10


void I2C_Start(void);
void I2C_End(void);
void i2c_write_byte(unsigned char temp);
unsigned char i2c_read_byte(void);
unsigned char i2c_read_keep_byte(void);


u8 FT6336_WR_Reg(u8 reg,u8 *buf,u8 len);
void FT6336_RD_Reg(u8 reg,u8 *buf,u8 len);
void FT6336_Init(void);

unsigned char i2c_read_addr_byte(unsigned char device_addr,unsigned char read_addr);
void i2c_read_addr_str(unsigned char device_addr,unsigned char read_addr,unsigned char read_amount,unsigned char *read_buf);
void i2c_write_addr_byte(unsigned char device_addr,unsigned char write_addr,unsigned char write_dat);
unsigned int i2c_read_addr_int(unsigned char device_addr,unsigned char read_addr);
void i2c_write_addr_str(unsigned char device_addr,unsigned char write_addr,unsigned char write_amount,unsigned char *write_buf);

#endif

//**************************************************************************
//I2C总线部分
//**************************************************************************
#include "stm32f10x.h"
#include "i2c.h"
#include "delay.h"
#include "FT6336.h"

extern void delay_us(u32 nus);	 
extern void delay_ms(u16 nms);




/* 
**函数名：FT6236_Init
**传入参数：无
**返回值：无
**功能：初始化FT6236引脚
*/  
  void FT6336_Init(void)
{
	u8 temp,KK;
	GPIO_InitTypeDef GPIO_InitStructure;					

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);

	//PD10:数据SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PD10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz	
	GPIO_Init(GPIOD,&GPIO_InitStructure);					//初始化对应GPIOE		
	GPIO_SetBits(GPIOD,GPIO_Pin_10);							//PD10 输出高	
		/******  PD9:复位SCL  ***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//PE7	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz	
	GPIO_Init(GPIOD,&GPIO_InitStructure);					//初始化对应GPIOE		
	GPIO_SetBits(GPIOD,GPIO_Pin_9);							//PE.7 输出高
	/******  PE15:复位RST  ***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//PE15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz	
	GPIO_Init(GPIOE,&GPIO_InitStructure);					//初始化对应GPIOE		
	GPIO_SetBits(GPIOE,GPIO_Pin_15);							//PE.15 输出高
	/**********************PE14，中断INT*********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				//PE14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IPU; 	//设置上拉输入，否则数据通信异常。
	GPIO_Init(GPIOE,&GPIO_InitStructure);	
	//VDD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//PD8	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz	
	GPIO_Init(GPIOD,&GPIO_InitStructure);					//初始化对应GPIOE		
	GPIO_SetBits(GPIOD,GPIO_Pin_8);							//PD.8 输出高
	//GND
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	//PE13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz	
	GPIO_Init(GPIOE,&GPIO_InitStructure);					//初始化对应GPIOE		
	GPIO_ResetBits(GPIOE,GPIO_Pin_13);							//PE.13 输出低

	FT6336_RST_L();
	delay_ms(50);
	FT6336_RST_H();
	delay_ms(100);
	FT6336_SDA_H();
	delay_ms(10);
	FT6336_SCL_H();
	delay_ms(10);
	temp=0;
	FT6336_WR_Reg(FT_DEVIDE_MODE,&temp,1);	//进入正常操作模式 
//	i2c_write_addr_byte(0x38,FT_DEVIDE_MODE,temp);
 	temp=22;								//触摸有效值，22，越小越灵敏	
 	FT6336_WR_Reg(FT_ID_G_THGROUP,&temp,1);	//设置触摸有效值
	FT6336_RD_Reg(FT_ID_G_THGROUP,&KK,1);
 	temp=14;								//激活周期，不能小于12，最大14
 	FT6336_WR_Reg(FT_ID_G_PERIODACTIVE,&temp,1); 
	FT6336_RD_Reg(FT_ID_G_PERIODACTIVE,&KK,1);
/******************************************************/
}




//-------------------------------分割复制部分-----------------------
/****************************************************
* 函数名称 ：
* 功    能 ：单片机发送起始信号
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：
*****************************************************/
void I2C_Start(void)
{
	SDA_OUT();     //sda线输出
	FT6336_SDA_H();	  	  
	FT6336_SCL_H();
	delay_us(10);   //8通道时,5us即可,16通道时,由于拉电流下降,至少需要8us,为了保证没问题设10us
 	FT6336_SDA_L();//START:when i2c is high,DATA change form high to low 
	delay_us(5);
	FT6336_SCL_L();//钳住I2C总线，准备发送或接收数据 
	delay_us(10);
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机发送停止信号
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：
*****************************************************/
void I2C_End(void)
{
//	SDA_OUT();     		//sda线输出
//	FT6336_SDA_L();		
//	delay_us(5);	
//	FT6336_SCL_H();		//SCL最小高电平脉宽:0.6us		
////	delay_us(4);		//停止信号的最小建立时间:0.6us
////	FT6336_SDA_L();	
//	delay_us(10);
//	FT6336_SDA_H();		//SCL高电平期间，SDA的一个上升沿表示停止信号
//	delay_us(5);		
	
	SDA_OUT();     		//sda线输出
	FT6336_SCL_L();		//SCL最小高电平脉宽:0.6us		
	FT6336_SDA_L();		
	delay_us(5);	
	FT6336_SCL_H();		//SCL最小高电平脉宽:0.6us		
	delay_us(4);		//停止信号的最小建立时间:0.6us
	FT6336_SDA_H();		//SCL高电平期间，SDA的一个上升沿表示停止信号
	delay_us(5);	
}


/****************************************************
* 函数名称 ：
* 功    能 ：单片机发送应答信号
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：单片机读1B数据后发送一个应答信号
*****************************************************/
void FT6336_McuACK(void)							
{
	FT6336_SCL_L();
	SDA_OUT();     		//sda线输出	
	FT6336_SDA_L();
	delay_us(5);																	
	FT6336_SCL_H();		//SCL最小高电平脉宽:0.6us
	delay_us(10);
	FT6336_SCL_L();	//SCL最小低电平脉宽:1.2us
	delay_us(10);
	
	FT6336_SDA_H();
}


/****************************************************
* 函数名称 ：
* 功    能 ：单片机发送非应答信号
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：单片机读数据停止前发送一个非应答信号
*****************************************************/
void FT6336_McuNACK(void)
{
	FT6336_SCL_L();
	SDA_OUT();     				//sda线输出	
	FT6336_SDA_H();
	delay_us(5);																	
	FT6336_SCL_H();				//SCL最小高电平脉宽:0.6us
	delay_us(10);
	FT6336_SCL_L();			//SCL最小低电平脉宽:1.2us
	delay_us(10);
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机检查FT6236送来的应答信号
* 入口参数 ：无
* 返回参数 ：1，接收应答失败
			 0，接收应答成功
* 注意事项 ：单片机写1个地址/数据后检查
			 全局变量RevAckF:收到FT6236应答信号的标志位,为0表示收到
*****************************************************/
u8 FT6336_CheckAck(void)							
{
	u8 ucErrTime=0;
	u8 redata;
	
	FT6336_SDA_H();
//	delay_us(5);  
	SDA_IN();  //SDA设置为输入
	FT6336_SCL_H();			//使SDA上数据有效;SCL最小高电平脉宽:0.6us
	delay_us(10);
	while(Is_SDA_IN != 0)
	{	
		ucErrTime++;
		if(ucErrTime>250)		//无应答
		{
			I2C_End();	
			return 1;
		}
	}
	FT6336_SCL_L();
	delay_us(10);
//	SDA_IN();
//	FT6336_SDA_H();
	return 0;
	
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机向IIC总线发送1B的地址/数据
* 入口参数 ：待发送的1B地址/数据
* 返回参数 ：无
* 注意事项 ：不是一个完整的数据发送过程;送数的顺序是从高到低
*****************************************************/
void FT6336_WrOneByte(u8 dat)						
{
	u8 i;						
	SDA_OUT();     				//sda线输出	
	FT6336_SCL_L();				//拉低时钟开始数据传输
	for(i = 0; i < 8; i++)		//8位1B地址/数据的长度
	{
		if(dat & 0x80) 		
			FT6336_SDA_H();		//发送"1"		
		else
			FT6336_SDA_L();		//发送"0"
		
		delay_us(10);
		FT6336_SCL_H();			//使SDA上的数据有效
		delay_us(10);			//SCL最小高电平脉宽:0.6us							
		FT6336_SCL_L();			//SCL最小低电平脉宽:1.2us
		delay_us(10);
		dat <<= 1;				//发送数据左移1位,为下位发送准备	
	}
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机从IIC总线接收1B的数据
* 入口参数 ：无
* 返回参数 ：收到的1B数据
* 注意事项 ：不是一个完整的数据接收过程;从高到低的顺序接收数据
*****************************************************/
u8 FT6336_RdOneByte(void)						
{
	u8 i,dat = 0;				//接收数据位数和内容暂存单元

	SDA_IN();						//SDA设置为输入
	for(i = 0;i < 8;i++)
	{
		FT6336_SCL_L();
		delay_us(10);
		FT6336_SCL_H();
		delay_us(10);			//SCL最小低电平脉宽:1.2us
		dat <<= 1;
		if(Is_SDA_IN != 0)
			dat |= 0x01;
		delay_us(5);			//SCL最小低电平脉宽:1.2us
	}
//	delay_us(2);
//	FT6336_SDA_H();		
	return(dat);				//返回1B的数据
	
}
/****************************************************
* 向FT6336写入一次数据
* reg:起始寄存器地址
* buf:数据缓缓存区
* len:写数据长度
* 返回值:0,成功;1,失败.
*****************************************************/
u8 FT6336_WR_Reg(u8 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	I2C_Start();	 
	FT6336_WrOneByte(FT_CMD_WR);	//发送写命令 	 
	FT6336_CheckAck(); 	 										  		   
	FT6336_WrOneByte(reg);   	//发送低8位地址
	FT6336_CheckAck();  
	for(i=0;i<len;i++)
	{	   
		FT6336_WrOneByte(buf[i]);  	//发数据
		ret=FT6336_CheckAck();
		if(ret)break;  
	}
    I2C_End();					//产生一个停止条件	    
	return ret; 
	

}
/*****************************************************
* 从FT6336读出一次数据
* reg:起始寄存器地址
* buf:数据缓缓存区
* len:读数据长度
*****************************************************/
void FT6336_RD_Reg(u8 reg,u8 *buf,u8 len)
{
	u8 i; 
 	I2C_Start();	
 	FT6336_WrOneByte(FT_CMD_WR);   	//发送写命令 	 
	FT6336_CheckAck(); 	 										  		   
 	FT6336_WrOneByte(reg);   	//发送低8位地址
	FT6336_CheckAck();  
//  I2C_End();					//产生一个停止条件	 
 	I2C_Start();  	 	   
	FT6336_WrOneByte(FT_CMD_RD);   	//发送读命令		   
	FT6336_CheckAck();	  
	for(i=0;i<(len-1);i++)
	{	   
		buf[i] = FT6336_RdOneByte();		//读入1B数据到接收数据缓冲区中
		FT6336_McuACK();					//发送应答位	  
	}
	buf[i]  = FT6336_RdOneByte();	
	FT6336_McuNACK();						//n个字节读完,发送非应答位
  I2C_End();					//产生一个停止条件	  

} 
 



void FT6336U_start(void)
{
	SDA_OUT();     //sda???
	FT6336_SDA_H();	  	  
	FT6336_SCL_H();
	delay_us(10);   //8???,5us??,16???,???????,????8us,????????10us
 	FT6336_SDA_L();//START:when i2c is high,DATA change form high to low 
	delay_us(10);
	FT6336_SCL_L();//??I2C??,????????? 
}

void FT6336U_end(void)
{
	SDA_OUT();//sda???
	FT6336_SCL_L();
	FT6336_SDA_L();//STOP:when i2c is high DATA change form low to high
 	delay_us(10);
	FT6336_SCL_H(); 
	FT6336_SDA_H();//??I2C??????
	delay_us(10);							   	
}

void i2c_write_byte(unsigned char txd)
{			   	
	u8 t,i2c_sda;   
	SDA_OUT(); 	    
	FT6336_SCL_L();//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
//		i2c_sda=(txd&0x80)>>7;
 
		if(txd&0x80)
			FT6336_SDA_H();
		else 
			FT6336_SDA_L();
		
		txd<<=1; 	
		
		delay_us(10);   //对TEA5767这三个延时都是必须的
		FT6336_SCL_H();
		delay_us(10); 
		FT6336_SCL_L();	
		delay_us(10);
	}	      
	FT6336_SDA_H();
	delay_us(10);
	SDA_IN();	   
	FT6336_SCL_H();
	delay_us(10);			 
	//	while(Is_SDA_IN);
	for(t=0;t<100;t++)//延时100us等待
	{
		if(Is_SDA_IN != 0)
			delay_us(1);
		else
			break;
	}
	FT6336_SCL_L();

	//----------------------------错误提示--------------------
	if(t >= 100)
	//string_normal(DIS_COL_BOUNDARY+1,UP_START+1,Red,"I2C_NCPL ERROR!");
	;
}

unsigned char i2c_read_byte(void)    //MCU无应答
{
	unsigned char i,receive=0;
	
	SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		FT6336_SCL_L(); 
		delay_us(10);
		FT6336_SCL_H();
		receive<<=1;
		if(Is_SDA_IN != 0) 
			receive |= 0x01;   
		delay_us(10); 
	}					 

	FT6336_SCL_L();
	SDA_OUT();
	FT6336_SDA_H();
	delay_us(10);
	FT6336_SCL_H();
	delay_us(10);
	FT6336_SCL_L();

	FT6336_SDA_H();  
	return receive;

}

unsigned char i2c_read_keep_byte(void)    //MCU有应答
{
	unsigned char i,receive=0;
	
	SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		FT6336_SCL_L(); 
		delay_us(10);
		FT6336_SCL_H();
		receive<<=1;
		if(Is_SDA_IN != 0) 
			receive |= 0x01;   
		delay_us(10); 
	}					 

	FT6336_SCL_L();
	SDA_OUT();
	FT6336_SDA_L();
	delay_us(10);
	FT6336_SCL_H();
	delay_us(10);
	FT6336_SCL_L();

	FT6336_SDA_H();  
	return receive;
}

unsigned char i2c_read_addr_byte(unsigned char device_addr,unsigned char read_addr)
{
	unsigned char dat;
	FT6336U_start();
	i2c_write_byte(device_addr<<1);
	i2c_write_byte(read_addr);
	FT6336U_end();

	FT6336U_start();
	i2c_write_byte((device_addr<<1) | 0x01);
	dat=i2c_read_byte();
	FT6336U_end();
	return(dat);
}

void i2c_read_addr_str(unsigned char device_addr,unsigned char read_addr,unsigned char read_amount,unsigned char *read_buf)
{
//	uchar dat;
	unsigned char i;
	FT6336U_start();
	i2c_write_byte(device_addr<<1);
	i2c_write_byte(read_addr);
	FT6336U_end();

	FT6336U_start();
	i2c_write_byte((device_addr<<1) | 0x01);

	for(i=0;i<read_amount-1;i++)
	{
		read_buf[i] = i2c_read_keep_byte();	
	}
	read_buf[i] = i2c_read_byte();
	FT6336U_end();
}


void i2c_write_addr_byte(unsigned char device_addr,unsigned char write_addr,unsigned char write_dat)
{
	FT6336U_start();
	i2c_write_byte(device_addr<<1);
	i2c_write_byte(write_addr);
	i2c_write_byte(write_dat);
	FT6336U_end();
	delay_ms(2);
}

unsigned int i2c_read_addr_int(unsigned char device_addr,unsigned char read_addr)
{
	unsigned char read_buf[2];
	i2c_read_addr_str(device_addr,read_addr,2,read_buf);
	return (read_buf[0]<<8)|read_buf[1];
}

void i2c_write_addr_str(unsigned char device_addr,unsigned char write_addr,unsigned char write_amount,unsigned char *write_buf)
{
	unsigned char i;
	FT6336U_start();
	i2c_write_byte(device_addr<<1);
	i2c_write_byte(write_addr);
	for(i=0;i<write_amount;i++)
	{
		i2c_write_byte(write_buf[i]);
	}
	FT6336U_end();
	delay_ms(2);
}

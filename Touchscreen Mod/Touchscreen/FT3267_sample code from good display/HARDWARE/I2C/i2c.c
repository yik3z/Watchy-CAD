//**************************************************************************
//I2C���߲���
//**************************************************************************
#include "stm32f10x.h"
#include "i2c.h"
#include "delay.h"
#include "FT6336.h"

extern void delay_us(u32 nus);	 
extern void delay_ms(u16 nms);




/* 
**��������FT6236_Init
**�����������
**����ֵ����
**���ܣ���ʼ��FT6236����
*/  
  void FT6336_Init(void)
{
	u8 temp,KK;
	GPIO_InitTypeDef GPIO_InitStructure;					

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);

	//PD10:����SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PD10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz	
	GPIO_Init(GPIOD,&GPIO_InitStructure);					//��ʼ����ӦGPIOE		
	GPIO_SetBits(GPIOD,GPIO_Pin_10);							//PD10 �����	
		/******  PD9:��λSCL  ***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//PE7	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz	
	GPIO_Init(GPIOD,&GPIO_InitStructure);					//��ʼ����ӦGPIOE		
	GPIO_SetBits(GPIOD,GPIO_Pin_9);							//PE.7 �����
	/******  PE15:��λRST  ***************************************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//PE15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz	
	GPIO_Init(GPIOE,&GPIO_InitStructure);					//��ʼ����ӦGPIOE		
	GPIO_SetBits(GPIOE,GPIO_Pin_15);							//PE.15 �����
	/**********************PE14���ж�INT*********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				//PE14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IPU; 	//�����������룬��������ͨ���쳣��
	GPIO_Init(GPIOE,&GPIO_InitStructure);	
	//VDD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//PD8	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz	
	GPIO_Init(GPIOD,&GPIO_InitStructure);					//��ʼ����ӦGPIOE		
	GPIO_SetBits(GPIOD,GPIO_Pin_8);							//PD.8 �����
	//GND
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	//PE13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz	
	GPIO_Init(GPIOE,&GPIO_InitStructure);					//��ʼ����ӦGPIOE		
	GPIO_ResetBits(GPIOE,GPIO_Pin_13);							//PE.13 �����

	FT6336_RST_L();
	delay_ms(50);
	FT6336_RST_H();
	delay_ms(100);
	FT6336_SDA_H();
	delay_ms(10);
	FT6336_SCL_H();
	delay_ms(10);
	temp=0;
	FT6336_WR_Reg(FT_DEVIDE_MODE,&temp,1);	//������������ģʽ 
//	i2c_write_addr_byte(0x38,FT_DEVIDE_MODE,temp);
 	temp=22;								//������Чֵ��22��ԽСԽ����	
 	FT6336_WR_Reg(FT_ID_G_THGROUP,&temp,1);	//���ô�����Чֵ
	FT6336_RD_Reg(FT_ID_G_THGROUP,&KK,1);
 	temp=14;								//�������ڣ�����С��12�����14
 	FT6336_WR_Reg(FT_ID_G_PERIODACTIVE,&temp,1); 
	FT6336_RD_Reg(FT_ID_G_PERIODACTIVE,&KK,1);
/******************************************************/
}




//-------------------------------�ָ�Ʋ���-----------------------
/****************************************************
* �������� ��
* ��    �� ����Ƭ��������ʼ�ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��
*****************************************************/
void I2C_Start(void)
{
	SDA_OUT();     //sda�����
	FT6336_SDA_H();	  	  
	FT6336_SCL_H();
	delay_us(10);   //8ͨ��ʱ,5us����,16ͨ��ʱ,�����������½�,������Ҫ8us,Ϊ�˱�֤û������10us
 	FT6336_SDA_L();//START:when i2c is high,DATA change form high to low 
	delay_us(5);
	FT6336_SCL_L();//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(10);
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ������ֹͣ�ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��
*****************************************************/
void I2C_End(void)
{
//	SDA_OUT();     		//sda�����
//	FT6336_SDA_L();		
//	delay_us(5);	
//	FT6336_SCL_H();		//SCL��С�ߵ�ƽ����:0.6us		
////	delay_us(4);		//ֹͣ�źŵ���С����ʱ��:0.6us
////	FT6336_SDA_L();	
//	delay_us(10);
//	FT6336_SDA_H();		//SCL�ߵ�ƽ�ڼ䣬SDA��һ�������ر�ʾֹͣ�ź�
//	delay_us(5);		
	
	SDA_OUT();     		//sda�����
	FT6336_SCL_L();		//SCL��С�ߵ�ƽ����:0.6us		
	FT6336_SDA_L();		
	delay_us(5);	
	FT6336_SCL_H();		//SCL��С�ߵ�ƽ����:0.6us		
	delay_us(4);		//ֹͣ�źŵ���С����ʱ��:0.6us
	FT6336_SDA_H();		//SCL�ߵ�ƽ�ڼ䣬SDA��һ�������ر�ʾֹͣ�ź�
	delay_us(5);	
}


/****************************************************
* �������� ��
* ��    �� ����Ƭ������Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ����Ƭ����1B���ݺ���һ��Ӧ���ź�
*****************************************************/
void FT6336_McuACK(void)							
{
	FT6336_SCL_L();
	SDA_OUT();     		//sda�����	
	FT6336_SDA_L();
	delay_us(5);																	
	FT6336_SCL_H();		//SCL��С�ߵ�ƽ����:0.6us
	delay_us(10);
	FT6336_SCL_L();	//SCL��С�͵�ƽ����:1.2us
	delay_us(10);
	
	FT6336_SDA_H();
}


/****************************************************
* �������� ��
* ��    �� ����Ƭ�����ͷ�Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ����Ƭ��������ֹͣǰ����һ����Ӧ���ź�
*****************************************************/
void FT6336_McuNACK(void)
{
	FT6336_SCL_L();
	SDA_OUT();     				//sda�����	
	FT6336_SDA_H();
	delay_us(5);																	
	FT6336_SCL_H();				//SCL��С�ߵ�ƽ����:0.6us
	delay_us(10);
	FT6336_SCL_L();			//SCL��С�͵�ƽ����:1.2us
	delay_us(10);
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ�����FT6236������Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ��1������Ӧ��ʧ��
			 0������Ӧ��ɹ�
* ע������ ����Ƭ��д1����ַ/���ݺ���
			 ȫ�ֱ���RevAckF:�յ�FT6236Ӧ���źŵı�־λ,Ϊ0��ʾ�յ�
*****************************************************/
u8 FT6336_CheckAck(void)							
{
	u8 ucErrTime=0;
	u8 redata;
	
	FT6336_SDA_H();
//	delay_us(5);  
	SDA_IN();  //SDA����Ϊ����
	FT6336_SCL_H();			//ʹSDA��������Ч;SCL��С�ߵ�ƽ����:0.6us
	delay_us(10);
	while(Is_SDA_IN != 0)
	{	
		ucErrTime++;
		if(ucErrTime>250)		//��Ӧ��
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
* �������� ��
* ��    �� ����Ƭ����IIC���߷���1B�ĵ�ַ/����
* ��ڲ��� �������͵�1B��ַ/����
* ���ز��� ����
* ע������ ������һ�����������ݷ��͹���;������˳���ǴӸߵ���
*****************************************************/
void FT6336_WrOneByte(u8 dat)						
{
	u8 i;						
	SDA_OUT();     				//sda�����	
	FT6336_SCL_L();				//����ʱ�ӿ�ʼ���ݴ���
	for(i = 0; i < 8; i++)		//8λ1B��ַ/���ݵĳ���
	{
		if(dat & 0x80) 		
			FT6336_SDA_H();		//����"1"		
		else
			FT6336_SDA_L();		//����"0"
		
		delay_us(10);
		FT6336_SCL_H();			//ʹSDA�ϵ�������Ч
		delay_us(10);			//SCL��С�ߵ�ƽ����:0.6us							
		FT6336_SCL_L();			//SCL��С�͵�ƽ����:1.2us
		delay_us(10);
		dat <<= 1;				//������������1λ,Ϊ��λ����׼��	
	}
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ����IIC���߽���1B������
* ��ڲ��� ����
* ���ز��� ���յ���1B����
* ע������ ������һ�����������ݽ��չ���;�Ӹߵ��͵�˳���������
*****************************************************/
u8 FT6336_RdOneByte(void)						
{
	u8 i,dat = 0;				//��������λ���������ݴ浥Ԫ

	SDA_IN();						//SDA����Ϊ����
	for(i = 0;i < 8;i++)
	{
		FT6336_SCL_L();
		delay_us(10);
		FT6336_SCL_H();
		delay_us(10);			//SCL��С�͵�ƽ����:1.2us
		dat <<= 1;
		if(Is_SDA_IN != 0)
			dat |= 0x01;
		delay_us(5);			//SCL��С�͵�ƽ����:1.2us
	}
//	delay_us(2);
//	FT6336_SDA_H();		
	return(dat);				//����1B������
	
}
/****************************************************
* ��FT6336д��һ������
* reg:��ʼ�Ĵ�����ַ
* buf:���ݻ�������
* len:д���ݳ���
* ����ֵ:0,�ɹ�;1,ʧ��.
*****************************************************/
u8 FT6336_WR_Reg(u8 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	I2C_Start();	 
	FT6336_WrOneByte(FT_CMD_WR);	//����д���� 	 
	FT6336_CheckAck(); 	 										  		   
	FT6336_WrOneByte(reg);   	//���͵�8λ��ַ
	FT6336_CheckAck();  
	for(i=0;i<len;i++)
	{	   
		FT6336_WrOneByte(buf[i]);  	//������
		ret=FT6336_CheckAck();
		if(ret)break;  
	}
    I2C_End();					//����һ��ֹͣ����	    
	return ret; 
	

}
/*****************************************************
* ��FT6336����һ������
* reg:��ʼ�Ĵ�����ַ
* buf:���ݻ�������
* len:�����ݳ���
*****************************************************/
void FT6336_RD_Reg(u8 reg,u8 *buf,u8 len)
{
	u8 i; 
 	I2C_Start();	
 	FT6336_WrOneByte(FT_CMD_WR);   	//����д���� 	 
	FT6336_CheckAck(); 	 										  		   
 	FT6336_WrOneByte(reg);   	//���͵�8λ��ַ
	FT6336_CheckAck();  
//  I2C_End();					//����һ��ֹͣ����	 
 	I2C_Start();  	 	   
	FT6336_WrOneByte(FT_CMD_RD);   	//���Ͷ�����		   
	FT6336_CheckAck();	  
	for(i=0;i<(len-1);i++)
	{	   
		buf[i] = FT6336_RdOneByte();		//����1B���ݵ��������ݻ�������
		FT6336_McuACK();					//����Ӧ��λ	  
	}
	buf[i]  = FT6336_RdOneByte();	
	FT6336_McuNACK();						//n���ֽڶ���,���ͷ�Ӧ��λ
  I2C_End();					//����һ��ֹͣ����	  

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
	FT6336_SCL_L();//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{              
//		i2c_sda=(txd&0x80)>>7;
 
		if(txd&0x80)
			FT6336_SDA_H();
		else 
			FT6336_SDA_L();
		
		txd<<=1; 	
		
		delay_us(10);   //��TEA5767��������ʱ���Ǳ����
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
	for(t=0;t<100;t++)//��ʱ100us�ȴ�
	{
		if(Is_SDA_IN != 0)
			delay_us(1);
		else
			break;
	}
	FT6336_SCL_L();

	//----------------------------������ʾ--------------------
	if(t >= 100)
	//string_normal(DIS_COL_BOUNDARY+1,UP_START+1,Red,"I2C_NCPL ERROR!");
	;
}

unsigned char i2c_read_byte(void)    //MCU��Ӧ��
{
	unsigned char i,receive=0;
	
	SDA_IN();//SDA����Ϊ����
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

unsigned char i2c_read_keep_byte(void)    //MCU��Ӧ��
{
	unsigned char i,receive=0;
	
	SDA_IN();//SDA����Ϊ����
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

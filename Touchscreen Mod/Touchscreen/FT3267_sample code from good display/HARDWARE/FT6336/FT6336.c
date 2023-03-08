#include "FT6336.h"
#include "delay.h"
#include "i2c.h"
#include "usart.h"



//-----------------------------------����Ѱ�ҵ�ַ-----------------------------------------------

/*
u16 check_addr;
u16 check_ok_times;
void i2c_write_byte_check(unsigned char txd)
{			   	
    u8 t;   
	SDA_OUT(); 	    
    i2c_scl=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        i2c_sda=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(10);   //��TEA5767��������ʱ���Ǳ����
		i2c_scl=1;
		delay_us(10); 
		i2c_scl=0;	
		delay_us(10);
    }	      
	i2c_sda=1;delay_us(10);
	SDA_IN();	   
	i2c_scl=1;delay_us(10);			 
//	while(i2c_sda_rd);
	for(t=0;t<100;t++)//��ʱ100us�ȴ�
		if(i2c_sda_rd)
			delay_us(1);
		else
			break;
	i2c_scl=0;
	
	//----------------------------�����ɹ������ʾ--------------------
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

//��һ����
u8 FT6336_read_touch1_event(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_EVENT)>>4;
}

u8 FT6336_read_touch1_id(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_ID)>>4;
}

//���½�Ϊ�������,ˮƽY��,��ֱX��
//Y�᷶Χ0~459
//X�᷶Χ0~319
//touch1��touch2����˳��Ϊ���������,��ID��Ϊ���µ�ʱ���Ⱥ���,�ñ�ſɴ����µ���ָ
//�ڵ�һ�㰴�º�ID��Ϊ0,�¼�Ϊ8,��̧���ID�����걣�������ֵ,�¼�Ϊ4,��ʱ�ڶ���������һֱΪFF
//�ڵ�һ�㰴��ǰ���°��ڶ�����,ID��Ϊ1,�¼�Ϊ8,̧���ȫ�ָ�FF,�����ڶ�����ĽӴ�������ڵ�һ����ʱ,
//��������ID������ǰ�������һ��������洢��ַ,��һ����̧���,�ڶ����������IDΪ1
//���µ�������ʱ,���������������������ԱȺ�,ֱ�����������С�ĵ�
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

//�ڶ�����
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

//����ʾ�⣨FPC���£�
////y��////////    //264x176
							//
							//
							//x��
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
	FT6336_RD_Reg(0x02,&sta,1);//��ȡ�������״̬  
  touch_count=sta;	
	FT6336_RD_Reg(0x01,&gestid,1);//��ȡ�������״̬
	
 	if(sta & 0x0f)	//�ж��Ƿ��д����㰴�£�0x02�Ĵ����ĵ�4λ��ʾ��Ч�������
 	{
 		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));	//~(0xFF << (sta & 0x0F))����ĸ���ת��Ϊ�����㰴����Ч��־
 		for(i=0;i<2;i++)	                                //�ֱ��жϴ�����1-5�Ƿ񱻰���
 		{
 			if(TPR_Structure.TouchSta & (1<<i))			    //��ȡ����������
 			{											    //���������ȡ��Ӧ��������������
 				FT6336_RD_Reg(FT6236_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
				TPR_Structure.x[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
				TPR_Structure.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
 				if((buf[0]&0XC0)!=0X80)
 				{
					TPR_Structure.x[i]=TPR_Structure.y[i]=0;//������contact�¼�������Ϊ��Ч	
					TPR_Structure.TouchSta &=0xe0;	//�����������Ч���
					return;
				}
 			}
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;     //�������±��
 	}
 	else
 	{
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN) 	//֮ǰ�Ǳ����µ�
 			TPR_Structure.TouchSta &= ~0x80;        //�����ɿ����	
 		else
 		{
 			TPR_Structure.x[0] = 0;
 			TPR_Structure.y[0] = 0;
 			TPR_Structure.TouchSta &=0xe0;	//�����������Ч���
 		}
 	}
}


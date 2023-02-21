#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"

void delay_xms(unsigned int xms)
{
	unsigned int i;
	while(xms--)
	{
		i=12000;
		while(i--);
	}
}

void EPD_W21_Init(void)
{
	EPD_W21_RST_0;		// Module reset
	delay_xms(10);//At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10);//At least 10ms delay 
}

void EPD_Display(unsigned char *Image)
{
    unsigned int Width, Height,i,j;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_W21_WriteCMD(0x10);
    for (j = 0; j < Height; j++) {
        for ( i = 0; i < Width; i++) {
            EPD_W21_WriteDATA(0x00);
        }
    }

    EPD_W21_WriteCMD(0x13);
    for ( j = 0; j < Height; j++) {
        for ( i = 0; i < Width; i++) {
           EPD_W21_WriteDATA(Image[i + j * Width]);
        }
    }
		EPD_W21_WriteCMD(0x12);			//DISPLAY REFRESH 	
		delay_xms(1);	    //!!!The delay here is necessary, 200uS at least!!!     
		lcd_chkstatus();
}
//UC8151D
void EPD_init(void)
{	
	  EPD_W21_Init();
	  delay_xms(100);					//reset IC and select BUS 

		EPD_W21_WriteCMD(0x00);        			//panel setting
    EPD_W21_WriteDATA (0xDf);
		EPD_W21_WriteDATA (0x0e);	
	
		EPD_W21_WriteCMD(0x4D);        			//FITIinternal code
    EPD_W21_WriteDATA (0x55);

		EPD_W21_WriteCMD(0xaa);        			
    EPD_W21_WriteDATA (0x0f);
		
		EPD_W21_WriteCMD(0xE9);        			
    EPD_W21_WriteDATA (0x02);

		EPD_W21_WriteCMD(0xb6);        			
    EPD_W21_WriteDATA (0x11);
		
		EPD_W21_WriteCMD(0xF3);        			
    EPD_W21_WriteDATA (0x0a);     

		EPD_W21_WriteCMD(0x61);			//resolution setting
    EPD_W21_WriteDATA (0xc8);
		EPD_W21_WriteDATA (0x00);		
    EPD_W21_WriteDATA (0xc8);    
		
		EPD_W21_WriteCMD(0x60);        	 //Tcon setting		
    EPD_W21_WriteDATA (0x00);
		
		EPD_W21_WriteCMD(0X50);						
		EPD_W21_WriteDATA(0x97);//
		
	  EPD_W21_WriteCMD(0XE3);						
		EPD_W21_WriteDATA(0x00);
		
		EPD_W21_WriteCMD(0x04);//Power on
		delay_xms(100);
		lcd_chkstatus();	}
	
void EPD_sleep(void)
{
		EPD_W21_WriteCMD(0X50);  //VCOM AND DATA INTERVAL SETTING			
		EPD_W21_WriteDATA(0xf7); //WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7	

		EPD_W21_WriteCMD(0X02);  	//power off
	  lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal
	  EPD_W21_WriteCMD(0X07);  	//deep sleep
		EPD_W21_WriteDATA(0xA5);
}



void EPD_full_display_Clear(void)
{
		unsigned int i,j;	
		for(j=0;j<2;j++)
		{
			EPD_W21_WriteCMD(0x10);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(0x00);  
			}  
			delay_xms(2);
		  EPD_W21_WriteCMD(0x13);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(0xff);  
			}  
			delay_xms(2);
		
			EPD_W21_WriteCMD(0x12);			//DISPLAY REFRESH 	
			delay_xms(100);	    //!!!The delay here is necessary, 200uS at least!!!     
			lcd_chkstatus();	
		}       //waiting for the electronic paper IC to release the idle signal

}

void lcd_chkstatus(void)
{
	while(!isEPD_W21_BUSY);   
                       
}

	void EPD_full_display(const unsigned char *old_data,const unsigned char *new_data,unsigned char mode)// mode0:Refresh picture1,mode1:Refresh picture2... ,mode2¡¢3:Clear
{		
		unsigned int i;
		
    if(mode==0)  //mode0:Refresh picture1
		{
			EPD_W21_WriteCMD(0x10);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(0xff);  
			}  
			delay_xms(2);
		  EPD_W21_WriteCMD(0x13);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(new_data[i]);  
			}  
			delay_xms(2);
	  }
		
		else if(mode==1)  //mode0:Refresh picture2...
		{
			EPD_W21_WriteCMD(0x10);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(old_data[i]);  
			}  
			delay_xms(2);
		  EPD_W21_WriteCMD(0x13);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(new_data[i]);  
			}  
			delay_xms(2);
	  }
		
   else if(mode==2) 
		{
		  EPD_W21_WriteCMD(0x10);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(old_data[i]);  
			}  
			delay_xms(2);
			EPD_W21_WriteCMD(0x13);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(0xff);  
			}  
			delay_xms(2);
	  }	 
   else if(mode==3) 
		{
		  EPD_W21_WriteCMD(0x10);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(0xff); 
			}  
			delay_xms(2);
			EPD_W21_WriteCMD(0x13);
			for(i=0;i<5000;i++)	     
			{
					EPD_W21_WriteDATA(0xff);  
			}  
			delay_xms(2);
	  }

		EPD_W21_WriteCMD(0x12);			//DISPLAY REFRESH 	
		delay_xms(100);	    //!!!The delay here is necessary, 200uS at least!!!     
		lcd_chkstatus();
}
/***************************partial display function*************************************/

void EPD_partial_display(unsigned int x_start,unsigned int x_end,unsigned int y_start,unsigned int y_end ,const unsigned char *old_data,const unsigned char *new_data,unsigned char mode) //partial display
{
	  unsigned datas,i;
		EPD_W21_WriteCMD(0x00);        			//panel setting
    EPD_W21_WriteDATA (0xff);
		EPD_W21_WriteDATA (0x0e);		    
		lut1();

	  EPD_W21_WriteCMD(0x91);		//This command makes the display enter partial mode
		EPD_W21_WriteCMD(0x90);		//resolution setting
		EPD_W21_WriteDATA (x_start);   //x-start     
		EPD_W21_WriteDATA (x_end-1);	 //x-end	
	  EPD_W21_WriteDATA (0);	 //x Reserved	

		EPD_W21_WriteDATA (y_start);   //y-start    
    EPD_W21_WriteDATA (0);	 //y Reserved		
		EPD_W21_WriteDATA (y_end);	 //y-end	
		EPD_W21_WriteDATA (0x01);	
    datas=(x_end-x_start)*(y_end-y_start)/8;
		
		EPD_W21_WriteCMD(0x10);	       //writes Old data to SRAM for programming
		if(mode==0)
		{
			for(i=0;i<datas;i++)	     
			{
				EPD_W21_WriteDATA(0xff);  
			} 			
		}
		else
		{
			for(i=0;i<datas;i++)	     
			{
				EPD_W21_WriteDATA(old_data[i]);  
			} 
	  }
		EPD_W21_WriteCMD(0x13);				 //writes New data to SRAM.
		for(i=0;i<datas;i++)	     
		{
			EPD_W21_WriteDATA(new_data[i]);  
		} 
    	
		EPD_W21_WriteCMD(0x12);		 //DISPLAY REFRESH 		             
		delay_xms(100);     //!!!The delay here is necessary, 200uS at least!!!     
		lcd_chkstatus();
		
}





/////////////////////////////////////partial screen update LUT///////////////////////////////////////////
const unsigned char lut_vcomDC1[] ={	0x01,	0x04,	0x04,	0x03,	0x01,	0x01,	0x01,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x01,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	};
								
								
								
								
								
const unsigned char lut_ww1[] ={	0x01,	0x04,	0x04,	0x03,	0x01,	0x01,	0x01,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	};
								
								
								
								
const unsigned char lut_bw1[] ={	0x01,	0x84,	0x84,	0x83,	0x01,	0x01,	0x01,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	};
								
								
								
								
								
const unsigned char lut_wb1[] ={	0x01,	0x44,	0x44,	0x43,	0x01,	0x01,	0x01,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	};
								
								
								
								
								
const unsigned char lut_bb1[] ={	0x01,	0x04,	0x04,	0x03,	0x01,	0x01,	0x01,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	};




void lut1(void)
{
	unsigned int count;
	EPD_W21_WriteCMD(0x20);
	for(count=0;count<42;count++)	     
		{EPD_W21_WriteDATA(lut_vcomDC1[count]);}

	EPD_W21_WriteCMD(0x21);
	for(count=0;count<42;count++)	     
		{EPD_W21_WriteDATA(lut_ww1[count]);}   
	
	EPD_W21_WriteCMD(0x22);
	for(count=0;count<42;count++)	     
		{EPD_W21_WriteDATA(lut_bw1[count]);} 

	EPD_W21_WriteCMD(0x23);
	for(count=0;count<42;count++)	     
		{EPD_W21_WriteDATA(lut_wb1[count]);} 

	EPD_W21_WriteCMD(0x24);
	for(count=0;count<42;count++)	     
		{EPD_W21_WriteDATA(lut_bb1[count]);}   
}













/***********************************************************
						end file
***********************************************************/


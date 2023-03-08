#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_


#define EPD_WIDTH   200
#define EPD_HEIGHT  200

//EPD
void EPD_W21_Init(void);
void EPD_init(void);
void EPD_full_display(const unsigned char *old_data,const unsigned char *new_data,unsigned char mode);// mode0:Refresh picture1,mode1:Refresh picture2... ,mode2¡¢3:Clear
void EPD_partial_display(unsigned int x_start,unsigned int x_end,unsigned int y_start,unsigned int y_end ,const unsigned char *old_data,const unsigned char *new_data,unsigned char mode); //partial display
void lut(void);
void lut1(void);
void EPD_sleep(void);
void EPD_refresh(void);
void lcd_chkstatus(void);
void EPD_full_display_Clear(void);
//Display canvas function
void EPD_Display(unsigned char *Image); 



#endif
/***********************************************************
						end file
***********************************************************/



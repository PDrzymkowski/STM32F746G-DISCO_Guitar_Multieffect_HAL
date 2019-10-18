#include "reverb.h"

uint8_t room_size_reverb = 50;
char room_size_str_reverb[3];
double depth_reverb = 0.5;
char depth_str_reverb[3];
uint8_t pre_delay1_reverb = 10;
char pre_delay1_str_reverb[3];
uint8_t pre_delay2_reverb = 10;
char pre_delay2_str_reverb[3];
uint8_t is_active_reverb = NOT_ACTIVE;
extern uint8_t is_button_active;


void Display_Reverb_Window(void)
{
		BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	
		/* Przyciski */
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(ROOM_SIZE_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
		BSP_LCD_FillRect(ROOM_SIZE_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
		BSP_LCD_FillRect(DEPTH_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
		BSP_LCD_FillRect(DEPTH_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	
		BSP_LCD_FillRect(PRE_DELAY1_UP_BUTTON_XPOS,  PARAM2_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
		BSP_LCD_FillRect(PRE_DELAY1_DOWN_BUTTON_XPOS, PARAM2_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
		BSP_LCD_FillRect(PRE_DELAY2_UP_BUTTON_XPOS,  PARAM2_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
		BSP_LCD_FillRect(PRE_DELAY2_DOWN_BUTTON_XPOS, PARAM2_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
		
		BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);
	
	  /* Napisy */
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_SetFont(&Font24);
		BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"REVERB", LEFT_MODE);
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_DisplayStringAt(ROOM_SIZE_DOWN_BUTTON_XPOS+30, PARAM1_BUTTON_YPOS-20, (uint8_t *)"ROOM SIZE[%]:", LEFT_MODE);	
		BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+60, PARAM1_BUTTON_YPOS-20, (uint8_t *)"DEPTH:", LEFT_MODE);		
		BSP_LCD_DisplayStringAt(PRE_DELAY1_DOWN_BUTTON_XPOS+5, PARAM2_BUTTON_YPOS-20, (uint8_t *)"PRE DELAY 1[ms]:", LEFT_MODE);	
		BSP_LCD_DisplayStringAt(PRE_DELAY2_DOWN_BUTTON_XPOS+5, PARAM2_BUTTON_YPOS-20, (uint8_t *)"PRE DELAY 2[ms]:", LEFT_MODE);			  
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_DisplayStringAt(ROOM_SIZE_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	  BSP_LCD_DisplayStringAt(ROOM_SIZE_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);
	  BSP_LCD_DisplayStringAt(DEPTH_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	  BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);		
		BSP_LCD_DisplayStringAt(PRE_DELAY1_UP_BUTTON_XPOS+12,  PARAM2_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	  BSP_LCD_DisplayStringAt(PRE_DELAY1_DOWN_BUTTON_XPOS+12, PARAM2_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);
	  BSP_LCD_DisplayStringAt(PRE_DELAY2_UP_BUTTON_XPOS+12,  PARAM2_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	  BSP_LCD_DisplayStringAt(PRE_DELAY2_DOWN_BUTTON_XPOS+12, PARAM2_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);		
		
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
		
		Display_Current_Parametres_Reverb();
	 	BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		if(is_active_reverb == NOT_ACTIVE)
		{	
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
		}else if(is_active_reverb == ACTIVE)
		{
				BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
				BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
		}
}

void ReverbWindow_Touch_Detection(uint16_t x, uint16_t y)
{
			if((y >PARAM1_BUTTON_YPOS) && (y <PARAM1_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
		{
				/* Sprawdzenie przycisku zwiekszenia parametru Room Size */	
				if((x > ROOM_SIZE_UP_BUTTON_XPOS) && (x < ROOM_SIZE_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(room_size_reverb<100)
					{
						room_size_reverb +=1;
						Display_Current_Parametres_Reverb();
					}
				/* Sprawdzenie przycisku zmnieszenia parametru Room Size */	
				}else if((x > ROOM_SIZE_DOWN_BUTTON_XPOS) && (x < ROOM_SIZE_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(room_size_reverb>0)
					{
						room_size_reverb -=1;
						Display_Current_Parametres_Reverb();
					}
				/* Sprawdzenie przycisku zwiekszenia parametru Depth */		
				}	else if((x > DEPTH_UP_BUTTON_XPOS) && (x < DEPTH_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(depth_reverb<1)
					{
						depth_reverb +=0.01;
						Display_Current_Parametres_Reverb();
					} 
				/* Sprawdzenie przycisku zmniejszenia parametru Depth */		
				}else if((x > GAIN_DOWN_BUTTON_XPOS) && (x < GAIN_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(depth_reverb>0.00)
					{
						depth_reverb -=0.01;
						Display_Current_Parametres_Reverb();
					} 
				}
		 }else if((y >PARAM2_BUTTON_YPOS) && (y <PARAM2_BUTTON_YPOS+ VERSION_BUTTON_HEIGHT))
		{
				/* Sprawdzenie przycisku zwiekszenia parametru Pre Delay 1 */	
				if((x > PRE_DELAY1_UP_BUTTON_XPOS) && (x < PRE_DELAY1_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(pre_delay1_reverb<20)
					{
						pre_delay1_reverb +=1;
						Display_Current_Parametres_Reverb();
					}
				/* Sprawdzenie przycisku zmnieszenia parametru Pre Delay 1 */	
				}else if((x > PRE_DELAY1_DOWN_BUTTON_XPOS) && (x < PRE_DELAY1_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(pre_delay1_reverb>0)
					{
						pre_delay1_reverb -=1;
						Display_Current_Parametres_Reverb();
					}
				/* Sprawdzenie przycisku zwiekszenia parametru Pre Delay 2 */		
				}	else if((x > PRE_DELAY2_UP_BUTTON_XPOS) && (x < PRE_DELAY2_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(pre_delay2_reverb<20)
					{
						pre_delay2_reverb +=1;
						Display_Current_Parametres_Reverb();
					} 
				/* Sprawdzenie przycisku zmniejszenia parametru Pre Delay 2 */		
				}else if((x > PRE_DELAY2_DOWN_BUTTON_XPOS) && (x < PRE_DELAY2_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(pre_delay2_reverb>0)
					{
						pre_delay2_reverb -=1;
						Display_Current_Parametres_Reverb();
					} 
				}
			
				/* Sprawdzenie przycisku powrotu do menu */	
		}else if((y > BACK_BUTTON_YPOS) && (y < BACK_BUTTON_YPOS+ BACK_BUTTON_HEIGHT) &&
							    	(x > BACK_BUTTON_XPOS) && (x < BACK_BUTTON_XPOS+ BACK_BUTTON_WIDTH))
			{
					Display_MainWindow();
			/* Sprawdzenie przycisku wlaczenia efektu */					
			}else if((y > ON_OFF_BUTTON_YPOS) && (y < ON_OFF_BUTTON_YPOS + ON_OFF_BUTTON_HEIGHT) &&
							    	(x > ON_OFF_BUTTON_XPOS) && (x < ON_OFF_BUTTON_XPOS + ON_OFF_BUTTON_WIDTH))
			{
			//		HAL_Delay(30);
					Display_On_Off_Info_Reverb();
			}
	
}

void Display_Current_Parametres_Reverb(void)
{
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(ROOM_SIZE_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH+20, PARAM_BUTTON_HEIGHT);
	  BSP_LCD_FillRect(PRE_DELAY1_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM2_BUTTON_YPOS, PARAM_BUTTON_WIDTH+20, PARAM_BUTTON_HEIGHT);
	  BSP_LCD_FillRect(PRE_DELAY2_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM2_BUTTON_YPOS, PARAM_BUTTON_WIDTH+20, PARAM_BUTTON_HEIGHT);
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_SetFont(&Font24);
		sprintf(room_size_str_reverb, "%d", room_size_reverb);
	  BSP_LCD_DisplayStringAt(ROOM_SIZE_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+30, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)room_size_str_reverb, LEFT_MODE);
		sprintf(depth_str_reverb, "%.2f", depth_reverb);
		BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)depth_str_reverb, LEFT_MODE);
		sprintf(pre_delay1_str_reverb, "%d", pre_delay1_reverb);
	  BSP_LCD_DisplayStringAt(PRE_DELAY1_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+30, PARAM2_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)pre_delay1_str_reverb, LEFT_MODE);
		sprintf(pre_delay2_str_reverb, "%d", pre_delay2_reverb);
		BSP_LCD_DisplayStringAt(PRE_DELAY2_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+30, PARAM2_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)pre_delay2_str_reverb, LEFT_MODE);
}

void Display_On_Off_Info_Reverb(void)
{
	if(is_button_active==BUTTON_NOT_ACTIVE)
		{	
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		if(is_active_reverb == ACTIVE)
		{	
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
				is_active_reverb = NOT_ACTIVE;
		}else if(is_active_reverb == NOT_ACTIVE)
		{
				BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
				BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
				is_active_reverb= ACTIVE;	
		}
	} 
	
}

void Reverb(void)
{
		if(is_active_reverb == ACTIVE)
	{
		
	}
}

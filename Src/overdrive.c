#include "overdrive.h"
#include <stdlib.h>

double clip_value_overdrive = 0.5;
char clip_value_str_overdrive[3];
uint8_t is_active_overdrive = NOT_ACTIVE;
extern uint8_t is_button_active;



void Display_Overdrive_Window(void)
{
		BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	
		/* Przyciski */
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(CLIP_VALUE_UP_BUTTON_XPOS,  CLIP_VALUE_BUTTON_YPOS, CLIP_VALUE_BUTTON_WIDTH,  CLIP_VALUE_BUTTON_HEIGHT);
		BSP_LCD_FillRect(CLIP_VALUE_DOWN_BUTTON_XPOS, CLIP_VALUE_BUTTON_YPOS, CLIP_VALUE_BUTTON_WIDTH, CLIP_VALUE_BUTTON_HEIGHT);
		BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);
	
	  /* Napisy */
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_SetFont(&Font24);
		BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"OVERDRIVE", LEFT_MODE);
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_DisplayStringAt(CLIP_VALUE_DOWN_BUTTON_XPOS+35, CLIP_VALUE_BUTTON_YPOS-20, (uint8_t *)"CLIP VALUE:", LEFT_MODE);	
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_DisplayStringAt(CLIP_VALUE_UP_BUTTON_XPOS+12,  CLIP_VALUE_BUTTON_YPOS+( CLIP_VALUE_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	  BSP_LCD_DisplayStringAt(CLIP_VALUE_DOWN_BUTTON_XPOS+12, CLIP_VALUE_BUTTON_YPOS+(CLIP_VALUE_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);	
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
		
		Display_Current_Parametres_Overdrive();
	 
	 	BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		if(is_active_overdrive == NOT_ACTIVE)
		{	
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
		}else if(is_active_overdrive == ACTIVE)
		{
				BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
				BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
		}
}

void OverdriveWindow_Touch_Detection(uint16_t x, uint16_t y)
{
	
		if((y > CLIP_VALUE_BUTTON_YPOS) && (y < CLIP_VALUE_BUTTON_YPOS+ CLIP_VALUE_BUTTON_HEIGHT))
		{
				/* Sprawdzenie przycisku zwiekszenia progu przesterowania */	
				if((x > CLIP_VALUE_UP_BUTTON_XPOS) && (x < CLIP_VALUE_UP_BUTTON_XPOS + CLIP_VALUE_BUTTON_WIDTH))
				{
					if(clip_value_overdrive<1)
					{
						clip_value_overdrive +=0.01;
						Display_Current_Parametres_Overdrive();
					}
				/* Sprawdzenie przycisku zmniejszenia progu przesterowania */	
				}else if((x > CLIP_VALUE_DOWN_BUTTON_XPOS) && (x < CLIP_VALUE_DOWN_BUTTON_XPOS + CLIP_VALUE_BUTTON_WIDTH))
				{
				if(clip_value_overdrive>0.01)
					{
						clip_value_overdrive -=0.01;
						Display_Current_Parametres_Overdrive();
					}
				/* Sprawdzenie przycisku powrotu do menu */		
				}
		 }else if((y > BACK_BUTTON_YPOS) && (y < BACK_BUTTON_YPOS+ BACK_BUTTON_HEIGHT) &&
							    	(x > BACK_BUTTON_XPOS) && (x < BACK_BUTTON_XPOS+ BACK_BUTTON_WIDTH))
			{
					Display_MainWindow();
			/* Sprawdzenie przycisku wlaczenia efektu */					
			}else if((y > ON_OFF_BUTTON_YPOS) && (y < ON_OFF_BUTTON_YPOS + ON_OFF_BUTTON_HEIGHT) &&
							    	(x > ON_OFF_BUTTON_XPOS) && (x < ON_OFF_BUTTON_XPOS + ON_OFF_BUTTON_WIDTH))
			{
				//	HAL_Delay(30);
					Display_On_Off_Info_Overdrive();
			}
}

void Display_Current_Parametres_Overdrive(void)
{
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_SetFont(&Font24);
		sprintf(clip_value_str_overdrive, "%.2f", clip_value_overdrive);
		BSP_LCD_DisplayStringAt(CLIP_VALUE_DOWN_BUTTON_XPOS+CLIP_VALUE_BUTTON_WIDTH+20, CLIP_VALUE_BUTTON_YPOS+(CLIP_VALUE_BUTTON_HEIGHT/2)-8, (uint8_t *)clip_value_str_overdrive, LEFT_MODE);
}

void Display_On_Off_Info_Overdrive()
{
	if(is_button_active==BUTTON_NOT_ACTIVE)
		{	
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
			if(is_active_overdrive == ACTIVE)
			{	
					BSP_LCD_SetTextColor(LCD_COLOR_RED);
					BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
					is_active_overdrive = NOT_ACTIVE;
			}else if(is_active_overdrive == NOT_ACTIVE)
			{
					BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
					BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
					BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
					BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
					is_active_overdrive = ACTIVE;	
			}
		}	
}

// TODO Poprawic zakres
//TODO Dodac Gain?

void Overdrive(uint16_t *data_out, uint8_t count)
{
		if(is_active_overdrive == ACTIVE)
	{
			uint16_t n;
			uint16_t overdrive_temp = 500*clip_value_overdrive;
			int16_t curr_sample;
			int16_t out_sample;
		
			for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
			{			
				curr_sample = (int16_t) data_out[n];
				
				if(curr_sample <= -(2*overdrive_temp/3))
				{
					out_sample = - overdrive_temp;
					
				}else if(curr_sample <= -(overdrive_temp/3))
				{
					// TODO sprawdzic pukanie

				//	out_sample = (-overdrive_temp/3) + (4*out_sample) - 3*(out_sample*out_sample/overdrive_temp);
					out_sample = (overdrive_temp/3) + (4*out_sample) + 3*(out_sample*out_sample/overdrive_temp);
					
				} else if (curr_sample > (-overdrive_temp/3) && curr_sample < (overdrive_temp/3))
				{
					out_sample = 2*curr_sample;
				} else if(curr_sample <= 2*overdrive_temp/3)
				{

			/*		data_out[n]= (overdrive_temp/4) + (4*data_in[n]) - (3*data_in[n]*data_in[n]/overdrive_temp);
					out_sample = overdrive_temp*(  3 -  (2-3*out_sample/overdrive_temp)* (2-3*out_sample/overdrive_temp )  )/3;
					out_sample = overdrive_temp*(  3 -  (4 - 12*out_sample/overdrive_temp + 9*(out_sample*out_sample)/(overdrive_temp*overdrive_temp )  ))/3;
					out_sample = overdrive_temp*(  3 -  4 +  12*out_sample/overdrive_temp -  9*(out_sample*out_sample)/(overdrive_temp*overdrive_temp ) ) /3;
					out_sample = (-overdrive_temp + 12*out_sample - 9*out_sample*out_sample/overdrive_temp)/3; */
			
					out_sample = (-overdrive_temp/3) + (4*out_sample) - 3*(out_sample*out_sample/overdrive_temp);
				} else
				{
					out_sample = overdrive_temp;
				}
				
					data_out[n] = (uint16_t)out_sample;
			
		  } 
	}
} 	


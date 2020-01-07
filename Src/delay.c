/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : delay.c
  * @brief         : Efekt delay, dodanie echa sygnalu
  ******************************************************************************
  */
/* USER CODE END Header */

#include "delay.h"

uint16_t delay_delay = 500;
char delay_str_delay[3];
float gain_delay = 0.5;
char gain_str_delay[3];
uint8_t version_delay = SOI;
uint8_t is_active_delay = NOT_ACTIVE;
extern uint8_t is_button_active;


uint32_t Get_Parameter_Delay(uint8_t parameter)
{
	switch(parameter)
	{
		case 0:
			return (uint32_t) (delay_delay);
		case 1:
			return (uint32_t) (gain_delay*100);
		case 2:
			return (uint32_t) (version_delay);
		case 3:
			return (uint32_t) (is_active_delay);
	}
	return 0;
}

void Set_Parameters_Delay(uint16_t delay, float gain, uint8_t version, uint8_t is_active)
{
	delay_delay = delay;
	gain_delay = gain;
	version_delay = version;
	is_active_delay = is_active;
}

void Display_Delay_Window(void)
{
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);

	/* Przyciski */
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(DELAY_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DELAY_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	
	BSP_LCD_FillRect(DELAY_10_UP_BUTTON_XPOS,  DELAY_10_BUTTON_YPOS,  DELAY_10_BUTTON_WIDTH,  DELAY_10_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DELAY_10_DOWN_BUTTON_XPOS, DELAY_10_BUTTON_YPOS, DELAY_10_BUTTON_WIDTH, DELAY_10_BUTTON_HEIGHT);
	
	BSP_LCD_FillRect(GAIN_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(GAIN_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(VERSION_BUTTON_XPOS,  PARAM2_BUTTON_YPOS, VERSION_BUTTON_WIDTH, VERSION_BUTTON_HEIGHT);
	BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);

	/* Napisy */
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"DELAY", LEFT_MODE);
	
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(DELAY_DOWN_BUTTON_XPOS+38, PARAM1_BUTTON_YPOS-20, (uint8_t *)"DELAY[ms]:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+60, PARAM1_BUTTON_YPOS-20, (uint8_t *)"GAIN:", LEFT_MODE);		
	BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS-8, PARAM2_BUTTON_YPOS-20, (uint8_t *)"WERSJA:", LEFT_MODE);		
	
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(DELAY_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(DELAY_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(DELAY_10_UP_BUTTON_XPOS+2,  DELAY_10_BUTTON_YPOS+( DELAY_10_BUTTON_HEIGHT/3)-1, (uint8_t *)"+10", LEFT_MODE);
	BSP_LCD_DisplayStringAt(DELAY_10_DOWN_BUTTON_XPOS+2, DELAY_10_BUTTON_YPOS+(DELAY_10_BUTTON_HEIGHT/3)-1, (uint8_t *)"-10", LEFT_MODE);
	
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(GAIN_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);			

	if(version_delay == SOI)
	{
		BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"SOI", LEFT_MODE);	
	}else if(version_delay == NOI)
	{
		BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"NOI", LEFT_MODE);				
	}
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
	
	Display_Current_Parameters_Delay();
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	if(is_active_delay == NOT_ACTIVE)
	{	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
	}else if(is_active_delay == ACTIVE)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
	}
}

void DelayWindow_Touch_Detection(uint16_t x, uint16_t y)
{
	if((y > PARAM1_BUTTON_YPOS) && (y < PARAM1_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia parametru Delay */	
		if((x > DELAY_UP_BUTTON_XPOS) && (x < DELAY_UP_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(delay_delay<999)
			{
				delay_delay +=1;
				Display_Current_Parameters_Delay();
			}
		/* Sprawdzenie przycisku zmnieszenia parametru Delay */	
		}else if((x > DELAY_DOWN_BUTTON_XPOS) && (x < DELAY_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(delay_delay>0)
			{
				delay_delay -=1;
				Display_Current_Parameters_Delay();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Gain */		
		}else if((x > GAIN_UP_BUTTON_XPOS) && (x < GAIN_UP_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(gain_delay<0.99f)
			{
				gain_delay += 0.01f;
				Display_Current_Parameters_Delay();
			} 
		/* Sprawdzenie przycisku zmniejszenia parametru Gain */		
		}else if((x > GAIN_DOWN_BUTTON_XPOS) && (x < GAIN_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(gain_delay>0.01f)
			{
				gain_delay -= 0.01f;
				Display_Current_Parameters_Delay();
			} 
		}
	}else if((y > PARAM2_BUTTON_YPOS) && (y < PARAM2_BUTTON_YPOS+ VERSION_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zmiany wersji efektu */	
		if((x > VERSION_BUTTON_XPOS) && (x < VERSION_BUTTON_XPOS +VERSION_BUTTON_WIDTH))
		{
			if(is_button_active==BUTTON_NOT_ACTIVE)
			{	
				BSP_LCD_SetFont(&Font16);
				BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				if(version_delay == SOI)
				{
					BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"NOI", LEFT_MODE);			
					version_delay = NOI;
				}else if(version_delay == NOI)
				{
					BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"SOI", LEFT_MODE);			
					version_delay = SOI	;
				}
			}
		}
	}else if((y >DELAY_10_BUTTON_YPOS) && (y < DELAY_10_BUTTON_YPOS + DELAY_10_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia parametru Delay o 10 ms*/	
		if((x > DELAY_10_UP_BUTTON_XPOS) && (x < DELAY_10_UP_BUTTON_XPOS +  DELAY_10_BUTTON_WIDTH))
		{
			if(delay_delay<991)
			{
				delay_delay +=10;
				Display_Current_Parameters_Delay();
			}
		/* Sprawdzenie przycisku zmniejszenia parametru Delay o 10 ms*/	
		}else if((x > DELAY_10_DOWN_BUTTON_XPOS) && (x < DELAY_10_DOWN_BUTTON_XPOS +  DELAY_10_BUTTON_WIDTH))
		{
			if(delay_delay > 9)
			{
				delay_delay -=10;
				Display_Current_Parameters_Delay();
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
		Display_On_Off_Info_Delay();
	}
}


void Display_Current_Parameters_Delay(void)
{
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect(DELAY_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH+35, PARAM_BUTTON_HEIGHT);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetFont(&Font24);
	sprintf(delay_str_delay, "%d", delay_delay);
	BSP_LCD_DisplayStringAt(DELAY_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+30, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)delay_str_delay, LEFT_MODE);
	sprintf(gain_str_delay, "%.2f", gain_delay);
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)gain_str_delay, LEFT_MODE);
} 

void Display_On_Off_Info_Delay(void)
{
	if(is_button_active==BUTTON_NOT_ACTIVE)
		{	
			BSP_LCD_SetFont(&Font12);
			BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		if(is_active_delay == ACTIVE)
		{	
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
			is_active_delay = NOT_ACTIVE; 
		}else if(is_active_delay == NOT_ACTIVE)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
			is_active_delay= ACTIVE;	
		}
	} 
}
void Delay(uint16_t *data_in, uint16_t* data_out, uint32_t count)
{
	uint32_t delay_samples = (delay_delay*44100)/1000;
	uint16_t n = 0;
	int16_t curr_sample = 0;
	int16_t prev_sample = 0;
	int16_t out_sample = 0;

	if(is_active_delay == ACTIVE)
	{
		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
		{								
			curr_sample =  (int16_t)data_out[n];
			if(version_delay == SOI)
				prev_sample =  (int16_t)data_in[(n + AUDIO_BUFFER_SIZE - delay_samples) % AUDIO_BUFFER_SIZE];
			else
				prev_sample =  (int16_t)data_out[(n + AUDIO_BUFFER_SIZE - delay_samples) % AUDIO_BUFFER_SIZE];
			
			out_sample = (curr_sample + gain_delay*prev_sample)/(1+gain_delay);
			data_out[n] = (uint16_t) (out_sample);									
		} 
	}
}




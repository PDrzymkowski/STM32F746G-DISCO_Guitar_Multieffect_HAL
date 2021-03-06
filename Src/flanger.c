/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : flanger.c
  * @brief         : Efekt flanger, modulacja sygnalu z echem
  ******************************************************************************
  */
/* USER CODE END Header */

#include "flanger.h"

static uint8_t delay_flanger = 10;
static float rate_flanger = 0.5;
static float depth_flanger = 0.5;
static uint8_t version_flanger = SOI;
static uint8_t is_active_flanger = NOT_ACTIVE;
static char delay_str_flanger[3];
static char rate_str_flanger[3];
static char depth_str_flanger[3];
extern uint8_t is_button_active;
extern char current_info_text[40];


uint32_t Get_Parameter_Flanger(uint8_t parameter)
{
	switch(parameter)
	{
		case 0:
			return (uint32_t) (delay_flanger);
		case 1:
			return (uint32_t) (rate_flanger*100);
		case 2:
			return (uint32_t) (depth_flanger*100);
		case 3:
			return (uint32_t) (version_flanger);
		case 4:
			return (uint32_t) (is_active_flanger);
	}
	return 0;
}

void Set_Parameters_Flanger(uint8_t delay, float rate, float depth, uint8_t version, uint8_t is_active)
{
	delay_flanger = delay;
	rate_flanger = rate;
	depth_flanger = depth;
	version_flanger = version;
	is_button_active = is_active;
}

void Display_Flanger_Window(void)
{
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);

	/* Przyciski */
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(DELAY_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DELAY_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(GAIN_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(GAIN_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(RATE2_UP_BUTTON_XPOS,  PARAM2_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(RATE2_DOWN_BUTTON_XPOS, PARAM2_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(VERSION_BUTTON_XPOS,  PARAM2_BUTTON_YPOS, VERSION_BUTTON_WIDTH, VERSION_BUTTON_HEIGHT);
	BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);

	/* Napisy */
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"FLANGER", LEFT_MODE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(DELAY_DOWN_BUTTON_XPOS+38, PARAM1_BUTTON_YPOS-20, (uint8_t *)"DELAY[ms]:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+60, PARAM1_BUTTON_YPOS-20, (uint8_t *)"GAIN:", LEFT_MODE);		
	BSP_LCD_DisplayStringAt(RATE2_DOWN_BUTTON_XPOS+43, PARAM2_BUTTON_YPOS-20, (uint8_t *)"RATE[Hz]:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS-8, PARAM2_BUTTON_YPOS-20, (uint8_t *)"WERSJA:", LEFT_MODE);		
	
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(DELAY_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(DELAY_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);
	BSP_LCD_DisplayStringAt(GAIN_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);			
	BSP_LCD_DisplayStringAt(RATE2_UP_BUTTON_XPOS+12,  PARAM2_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(RATE2_DOWN_BUTTON_XPOS+12, PARAM2_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);	
	
	if(version_flanger == SOI)
	{
		BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"SOI", LEFT_MODE);	
	}else if(version_flanger== NOI)
	{
		BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"NOI", LEFT_MODE);				
	}
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
	
	Display_Current_Parameters_Flanger();
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	if(is_active_flanger == NOT_ACTIVE)
	{	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
	}else if(is_active_flanger == ACTIVE)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
	}
}

void FlangerWindow_Touch_Detection(uint16_t x, uint16_t y)
{
	if((y >PARAM1_BUTTON_YPOS) && (y <PARAM1_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia parametru Delay */	
		if((x > DELAY_UP_BUTTON_XPOS) && (x < DELAY_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(delay_flanger<20)
			{
				delay_flanger +=1;
				Display_Current_Parameters_Flanger();
			}
		/* Sprawdzenie przycisku zmnieszenia parametru Delay */	
		}else if((x > DELAY_DOWN_BUTTON_XPOS) && (x < DELAY_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
		if(delay_flanger>0)
			{
				delay_flanger -=1;
				Display_Current_Parameters_Flanger();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Gain */		
		}else if((x > GAIN_UP_BUTTON_XPOS) && (x < GAIN_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(depth_flanger<0.99f)
			{
				depth_flanger +=0.01f;
				Display_Current_Parameters_Flanger();
			} 
		/* Sprawdzenie przycisku zmniejszenia parametru Gain */		
		}else if((x > GAIN_DOWN_BUTTON_XPOS) && (x < GAIN_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(depth_flanger>0.01f)
			{
					depth_flanger -=0.01f;
					Display_Current_Parameters_Flanger();
			} 
		}
	 }else if((y >PARAM2_BUTTON_YPOS) && (y <PARAM2_BUTTON_YPOS+ VERSION_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zmiany wersji efektu */	
		if((x > VERSION_BUTTON_XPOS) && (x < VERSION_BUTTON_XPOS +VERSION_BUTTON_WIDTH))
		{
			if(is_button_active==BUTTON_NOT_ACTIVE)
			{	
				BSP_LCD_SetFont(&Font16);
				BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				if(version_flanger == SOI)
				{
					BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"NOI", LEFT_MODE);			
					version_flanger = NOI;
				}else if(version_flanger == NOI)
				{
					BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"SOI", LEFT_MODE);			
					version_flanger = SOI	;
				}
			}
			/* Sprawdzenie przycisku zwiekszenia parametru Rate */	
		}else if((x > RATE2_UP_BUTTON_XPOS) && (x < RATE2_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
			{
				if(rate_flanger<0.99f)
				{
					rate_flanger +=0.01f;
					Display_Current_Parameters_Flanger();
				} 
			/* Sprawdzenie przycisku zmniejszenia parametru Rate */		
			}else if((x > RATE2_DOWN_BUTTON_XPOS) && (x < RATE2_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
			{
				if(rate_flanger>0.01f)
				{
					rate_flanger -=0.01f;
					Display_Current_Parameters_Flanger();
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
		Display_On_Off_Info_Flanger();
	}
}

void Display_Current_Parameters_Flanger(void)
{
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect(DELAY_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH+20, PARAM_BUTTON_HEIGHT);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetFont(&Font24);
	
	sprintf(delay_str_flanger, "%d", delay_flanger);
	BSP_LCD_DisplayStringAt(DELAY_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+30, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)delay_str_flanger, LEFT_MODE);
	sprintf(depth_str_flanger, "%.2f", depth_flanger);
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)depth_str_flanger, LEFT_MODE);
	sprintf(rate_str_flanger, "%.2f", rate_flanger);
	BSP_LCD_DisplayStringAt(RATE2_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM2_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)rate_str_flanger, LEFT_MODE);
}

void Display_On_Off_Info_Flanger(void)
{
	if(is_button_active==BUTTON_NOT_ACTIVE)
	{	
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		if(is_active_flanger == ACTIVE)
		{	
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
			is_active_flanger = NOT_ACTIVE;
			sprintf(current_info_text, "Wylaczono efekt Flanger");
		}else if(is_active_flanger == NOT_ACTIVE)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
			is_active_flanger= ACTIVE;	
			sprintf(current_info_text, "Wlaczono efekt Flanger");
		}
	} 
}

void Flanger(uint16_t *data_in, uint16_t* data_out, uint32_t count)
{
	if(is_active_flanger == ACTIVE)
	{
		uint16_t max_delay = (delay_flanger * 44100)/ 1000;
		uint16_t flanger_period = 44100 / rate_flanger;
		uint16_t n;
		uint16_t delay_samples;
		static uint16_t flanger_n = 0;
		float frequency_change = (float)(rate_flanger)/44100;
		int16_t curr_sample;
		int16_t prev_sample;
		int16_t out_sample;	

			for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
			{								
				curr_sample = (int16_t)data_out[n];	
				delay_samples = 1 + (max_delay/2)*(1-arm_cos_f32(2* PI* frequency_change*flanger_n));
				
				if(version_flanger == SOI)
						prev_sample = data_in[(n + AUDIO_BUFFER_SIZE - delay_samples) % AUDIO_BUFFER_SIZE];
				else
					prev_sample = data_out[(n + AUDIO_BUFFER_SIZE - delay_samples) % AUDIO_BUFFER_SIZE];
	
				out_sample = (curr_sample + depth_flanger*prev_sample)/(1+depth_flanger);
				data_out[n] = (uint16_t) (out_sample);
				flanger_n = (flanger_n + flanger_period + 1) % flanger_period;
			}
	 }
}

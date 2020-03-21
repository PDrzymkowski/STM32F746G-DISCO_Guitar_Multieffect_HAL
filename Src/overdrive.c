/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : overdrive.c
  * @brief         : Efekt overdrive, przesterowanie sygnalu
  ******************************************************************************
  */
/* USER CODE END Header */

#include "overdrive.h"

static float clip_value_overdrive = 0.5;
static float gain_overdrive = 0.5;
static uint8_t is_active_overdrive = NOT_ACTIVE;
static char clip_value_str_overdrive[3];
static char gain_str_overdrive[3];
extern uint8_t is_button_active;
extern char current_info_text[40];


uint32_t Get_Parameter_Overdrive(uint8_t parameter)
{
	switch(parameter)
	{
		case 0:
			return (uint32_t) (clip_value_overdrive*100);
		case 1:
			return (uint32_t) (gain_overdrive*100);
		case 2:
			return (uint32_t) (is_active_overdrive);
	}
	return 0;
}

void Set_Parameters_Overdrive(float clip_value, float gain, uint8_t is_active)
{
	clip_value_overdrive = clip_value;
	gain_overdrive = gain;
	is_active_overdrive = is_active;
}

void Display_Overdrive_Window(void)
{
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);

	/* Przyciski */
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(CLIP_VALUE_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH,  PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(CLIP_VALUE_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(GAIN_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH,  PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(GAIN_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);

	/* Napisy */
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"OVERDRIVE", LEFT_MODE);
	
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(CLIP_VALUE_DOWN_BUTTON_XPOS+35, PARAM1_BUTTON_YPOS-20, (uint8_t *)"CLIP VALUE:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+60, PARAM1_BUTTON_YPOS-20, (uint8_t *)"GAIN:", LEFT_MODE);	
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(CLIP_VALUE_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(CLIP_VALUE_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(GAIN_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);	
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
	
	Display_Current_Parameters_Overdrive();
 
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
	if((y > PARAM1_BUTTON_YPOS) && (y < PARAM1_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia progu przesterowania */	
		if((x > CLIP_VALUE_UP_BUTTON_XPOS) && (x < CLIP_VALUE_UP_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(clip_value_overdrive<0.99f)
			{
				clip_value_overdrive += 0.01f;
				Display_Current_Parameters_Overdrive();
			}
		/* Sprawdzenie przycisku zmniejszenia progu przesterowania */	
		}else if((x > CLIP_VALUE_DOWN_BUTTON_XPOS) && (x < CLIP_VALUE_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
		if(clip_value_overdrive>0.01f)
			{
				clip_value_overdrive -= 0.01f;
				Display_Current_Parameters_Overdrive();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Gain */	
		}else if((x >GAIN_UP_BUTTON_XPOS) && (x < GAIN_UP_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(gain_overdrive<0.99f)
			{
				gain_overdrive += 0.01f;
				Display_Current_Parameters_Overdrive();
			}
		/* Sprawdzenie przycisku zmniejszenia parametru Gain */		
		}else if((x > GAIN_DOWN_BUTTON_XPOS) && (x < GAIN_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
		if(gain_overdrive>0.01f)
			{
				gain_overdrive -= 0.01f;
				Display_Current_Parameters_Overdrive();
			}
		}
		/* Sprawdzenie przycisku powrotu do menu */		
	}else if((y > BACK_BUTTON_YPOS) && (y < BACK_BUTTON_YPOS + BACK_BUTTON_HEIGHT) &&
								(x > BACK_BUTTON_XPOS) && (x < BACK_BUTTON_XPOS + BACK_BUTTON_WIDTH))
	{
			Display_MainWindow();
	/* Sprawdzenie przycisku wlaczenia efektu */					
	}else if((y > ON_OFF_BUTTON_YPOS) && (y < ON_OFF_BUTTON_YPOS + ON_OFF_BUTTON_HEIGHT) &&
								(x > ON_OFF_BUTTON_XPOS) && (x < ON_OFF_BUTTON_XPOS + ON_OFF_BUTTON_WIDTH))
	{
		Display_On_Off_Info_Overdrive();
	}
}

void Display_Current_Parameters_Overdrive(void)
{
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetFont(&Font24);
	sprintf(clip_value_str_overdrive, "%.2f", clip_value_overdrive);
	BSP_LCD_DisplayStringAt(CLIP_VALUE_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+20, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)clip_value_str_overdrive, LEFT_MODE);
	sprintf(gain_str_overdrive, "%.2f", gain_overdrive);
	BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+20, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)gain_str_overdrive, LEFT_MODE);
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
			sprintf(current_info_text, "Wylaczono efekt Overdrive");
		}else if(is_active_overdrive == NOT_ACTIVE)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
			is_active_overdrive = ACTIVE;	
			sprintf(current_info_text, "Wlaczono efekt Overdrive");
		}
	}	
}


void Overdrive(uint16_t *data_out, uint32_t count)
{
	if(is_active_overdrive == ACTIVE)
	{
		uint16_t n;
		uint16_t overdrive_temp = 3000*clip_value_overdrive;
		int16_t curr_sample;
		int16_t out_sample;

		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
		{			
			curr_sample = (int16_t) data_out[n];
			out_sample = curr_sample;
			if(curr_sample <= -(2*overdrive_temp/3))
			{
				out_sample = - overdrive_temp;
			}else if(curr_sample <= -(overdrive_temp/3))
			{
				out_sample = ((overdrive_temp/3) + (4*out_sample) + (3*(out_sample*out_sample/overdrive_temp)));	
			}else if (curr_sample > (-overdrive_temp/3) && curr_sample < (overdrive_temp/3))
			{
				out_sample = 2*curr_sample;
			}else if(curr_sample <= 2*overdrive_temp/3)
			{
				out_sample = ((-overdrive_temp/3) + (4*out_sample) - (3*(out_sample*out_sample/overdrive_temp)));
			}else
			{
				out_sample = overdrive_temp;
			}
			out_sample = out_sample*(1+(gain_overdrive*1.5f));
			data_out[n] = (uint16_t)out_sample;
		} 
	}
} 	


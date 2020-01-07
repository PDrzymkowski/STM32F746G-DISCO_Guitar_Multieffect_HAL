/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : pitch_shifter.c
  * @brief         : Efekt pitch shifter, przesuniecie dzwieku 
	*										  w wysokosci
  ******************************************************************************
  */
/* USER CODE END Header */

#include "pitch_shifter.h"
#include "math.h"


int8_t pitch_pitchshifter = 0;
float gain_pitchshifter = 0.5;
uint8_t mode_pitchshifter = REPLACE;
uint8_t is_active_pitchshifter = NOT_ACTIVE;
char gain_str_pitchshifter[3];
char pitch_str_pitchshifter[3];
extern uint8_t is_button_active;

int16_t data_pitch_shifter[AUDIO_BLOCK_SIZE] = {0};


uint32_t Get_Parameter_PitchShifter(uint8_t parameter)
{
	switch(parameter)
	{
		case 0:
			return (uint32_t) (pitch_pitchshifter);
		case 1:
			return (uint32_t)  (gain_pitchshifter*100);
		case 2:
			return (uint32_t) (mode_pitchshifter);
		case 3:
			return (uint32_t) (is_active_pitchshifter);
	}
	return 0;
}


void Set_Parametres_PitchShifter(int8_t pitch, float gain, uint8_t mode, uint8_t is_active)
{
	pitch_pitchshifter = pitch;
	gain_pitchshifter = gain;
	mode_pitchshifter = mode;
	is_active_pitchshifter = is_active;
}


void Display_PitchShifter_Window(void)
{
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	
	/* Przyciski */
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(PITCH_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(PITCH_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	if(mode_pitchshifter == ADD)
	{
		BSP_LCD_FillRect(GAIN_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
		BSP_LCD_FillRect(GAIN_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	}
	BSP_LCD_FillRect(MODE_BUTTON_XPOS,  PARAM2_BUTTON_YPOS, MODE_BUTTON_WIDTH, MODE_BUTTON_HEIGHT);
	BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);

	/* Napisy */
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(TITLE_XPOS-15, TITLE_YPOS, (uint8_t *)"PITCH SHIFTER", LEFT_MODE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(PITCH_DOWN_BUTTON_XPOS+38, PARAM1_BUTTON_YPOS-20, (uint8_t *)"PITCH:", LEFT_MODE);	
	if(mode_pitchshifter == ADD)
	{
		BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+60, PARAM1_BUTTON_YPOS-20, (uint8_t *)"GAIN:", LEFT_MODE);		
	}
	BSP_LCD_DisplayStringAt(MODE_BUTTON_XPOS+15, PARAM2_BUTTON_YPOS-20, (uint8_t *)"WERSJA:", LEFT_MODE);		
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(PITCH_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(PITCH_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);
	if(mode_pitchshifter == ADD)
	{
		BSP_LCD_DisplayStringAt(GAIN_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
		BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);		
		BSP_LCD_DisplayStringAt(MODE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(MODE_BUTTON_HEIGHT/3)-1, (uint8_t *)"ADD", LEFT_MODE);	
	}else if(mode_pitchshifter == REPLACE)
	{
		BSP_LCD_DisplayStringAt(MODE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(MODE_BUTTON_HEIGHT/3)-1, (uint8_t *)"REPLACE", LEFT_MODE);				
	}
	
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
	
	Display_Current_Parametres_PitchShifter();
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	if(is_active_pitchshifter == NOT_ACTIVE)
	{	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
	}else if(is_active_pitchshifter == ACTIVE)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
	}
}

void PitchShifterWindow_Touch_Detection(uint16_t x, uint16_t y)
{
	if((y >PARAM1_BUTTON_YPOS) && (y <PARAM1_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia parametru Pitch */	
		if((x > PITCH_UP_BUTTON_XPOS) && (x < PITCH_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(pitch_pitchshifter<12)
			{
				pitch_pitchshifter +=1;
				Display_Current_Parametres_PitchShifter();
			}
		/* Sprawdzenie przycisku zmnieszenia parametru Pitch */	
		}else if((x > PITCH_DOWN_BUTTON_XPOS) && (x < PITCH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(pitch_pitchshifter>-12)
			{
				pitch_pitchshifter -=1;
				Display_Current_Parametres_PitchShifter();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Gain */		
		}else if((x > GAIN_UP_BUTTON_XPOS) && (x < GAIN_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(mode_pitchshifter == ADD)
			{
				if(gain_pitchshifter<0.99f)
				{
					gain_pitchshifter +=0.01f;
					Display_Current_Parametres_PitchShifter();
				} 
			}
		/* Sprawdzenie przycisku zmniejszenia parametru Gain */		
		}else if((x > GAIN_DOWN_BUTTON_XPOS) && (x < GAIN_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(mode_pitchshifter == ADD)
			{
				if(gain_pitchshifter>0.01f)
				{
					gain_pitchshifter -=0.01f;
					Display_Current_Parametres_PitchShifter();
				} 
			}
		}
	}else if((y >PARAM2_BUTTON_YPOS) && (y <PARAM2_BUTTON_YPOS+ MODE_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zmiany trybu efektu */	
		if((x > MODE_BUTTON_XPOS) && (x < MODE_BUTTON_XPOS + MODE_BUTTON_WIDTH))
		{
			if(is_button_active==BUTTON_NOT_ACTIVE)
			{	
				BSP_LCD_SetFont(&Font16);
				BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				if(mode_pitchshifter == REPLACE)
				{		
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_FillRect(MODE_BUTTON_XPOS,  PARAM2_BUTTON_YPOS, MODE_BUTTON_WIDTH, MODE_BUTTON_HEIGHT);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(MODE_BUTTON_XPOS+30, PARAM2_BUTTON_YPOS+(MODE_BUTTON_HEIGHT/3)-1, (uint8_t *)"ADD", LEFT_MODE);		
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_FillRect(GAIN_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
					BSP_LCD_FillRect(GAIN_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
					BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
					BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+60, PARAM1_BUTTON_YPOS-20, (uint8_t *)"GAIN:", LEFT_MODE);		
					BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(GAIN_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
					BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);		
					mode_pitchshifter = ADD;
				}else if(mode_pitchshifter == ADD)
				{
					BSP_LCD_DisplayStringAt(MODE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(MODE_BUTTON_HEIGHT/3)-1, (uint8_t *)"REPLACE", LEFT_MODE);			
					mode_pitchshifter = REPLACE;
					BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
					BSP_LCD_FillRect(GAIN_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS-40, 190, 80);
				}
				Display_Current_Parametres_PitchShifter();
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
		Display_On_Off_Info_PitchShifter();
	}
}

void Display_Current_Parametres_PitchShifter(void)
{
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect(PITCH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH+25, PARAM_BUTTON_HEIGHT);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetFont(&Font24);
	sprintf(pitch_str_pitchshifter, "%d", pitch_pitchshifter);
	BSP_LCD_DisplayStringAt(PITCH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+20, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)pitch_str_pitchshifter, LEFT_MODE);
	if(mode_pitchshifter == ADD)
	{
		sprintf(gain_str_pitchshifter, "%.2f", gain_pitchshifter);
		BSP_LCD_DisplayStringAt(GAIN_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)gain_str_pitchshifter, LEFT_MODE);
	}
}

void Display_On_Off_Info_PitchShifter(void)
{
	if(is_button_active==BUTTON_NOT_ACTIVE)
	{	
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	if(is_active_pitchshifter == ACTIVE)
	{	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
		is_active_pitchshifter = NOT_ACTIVE;
	}else if(is_active_pitchshifter == NOT_ACTIVE)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
		is_active_pitchshifter = ACTIVE;	
		}
	} 
}

void PitchShifter(uint16_t *data, int32_t count)
{
	if(is_active_pitchshifter == ACTIVE)
	{
		int i,n;
		i = 0;
		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
		{
			data_pitch_shifter[i] = (int16_t)data[n];
			i++;
		}
			
		//PhaseVocoder(data_pitch_shifter, pitch_pitchshifter);
		
		i = 0;
		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
		{
		//	data[n] = (uint16_t) data_pitch_shifter[i];
			i++;
		}
	}
}

#include "pitch_shifter.h"
#include "math.h"


double pitch_pitchshifter = 0;
char pitch_str_pitchshifter[3];
uint8_t mode_pitchshifter = REPLACE;
double gain_pitchshifter = 0.5;
char gain_str_pitchshifter[3];
uint8_t is_active_pitchshifter = NOT_ACTIVE;
extern uint8_t is_button_active;


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
		BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"DELAY", LEFT_MODE);
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_DisplayStringAt(PITCH_DOWN_BUTTON_XPOS+38, PARAM1_BUTTON_YPOS-20, (uint8_t *)"DELAY[ms]:", LEFT_MODE);	
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
				}	else if((x > GAIN_UP_BUTTON_XPOS) && (x < GAIN_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(mode_pitchshifter == ADD)
					{
							if(gain_pitchshifter<1)
							{
								gain_pitchshifter +=0.01;
								Display_Current_Parametres_PitchShifter();
							} 
				}
				/* Sprawdzenie przycisku zmniejszenia parametru Gain */		
				}else if((x > GAIN_DOWN_BUTTON_XPOS) && (x < GAIN_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
					if(mode_pitchshifter == ADD)
					{
						if(gain_pitchshifter>0.01)
							{
								gain_pitchshifter -=0.01;
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
					HAL_Delay(30);
					Display_On_Off_Info_PitchShifter();
			}
	
}

void Display_Current_Parametres_PitchShifter(void)
{
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(PITCH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH+20, PARAM_BUTTON_HEIGHT);
	
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	  BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_SetFont(&Font24);
		sprintf(pitch_str_pitchshifter, "%.1f", pitch_pitchshifter);
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

void PitchShifter(uint16_t *data)
{
		if(is_active_pitchshifter == ACTIVE)
	{
		double alpha = pow(2, (double)(pitch_pitchshifter)/12);
		uint16_t N = AUDIO_BLOCK_SIZE;
		uint16_t Ra = N/4;
		uint16_t Rs = alpha*Ra;
		//TODO okno hanninga i omega
	//	h = hann(2*N+1);       % okno hanninga (symetryczne)
	// h = h(2:2:end);
		// Omega = ((0:N-1)*2*pi/N)';
	}
}

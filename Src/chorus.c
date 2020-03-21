/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : chorus.c
  * @brief         : Efekt chorus, podwojna modualcja sygnalu z 
												echem, choralny efekt 
  ******************************************************************************
  */
/* USER CODE END Header */

#include "chorus.h"
#include "arm_math.h"
#include "arm_common_tables.h"

static uint8_t delay_chorus = 5;
static float rate1_chorus = 0.2;
static float rate2_chorus = 0.2;
static float depth1_chorus = 0.5;
static float depth2_chorus = 0.5;
static uint8_t version_chorus = SOI;
static uint8_t is_active_chorus = NOT_ACTIVE;
static char delay_str_chorus[3];
static char  rate1_str_chorus[3];
static char rate2_str_chorus[3];
static char depth1_str_chorus[3];
static char depth2_str_chorus[3];
extern uint8_t is_button_active;
extern char current_info_text[40];


uint32_t Get_Parameter_Chorus(uint8_t parameter)
{
	switch(parameter)
	{
		case 0:
			return (uint32_t) delay_chorus;
		case 1:
			return (uint32_t) (rate1_chorus*100);
		case 2:
			return (uint32_t) (rate2_chorus*100);
		case 3:
			return (uint32_t) (depth1_chorus*100);
		case 4:
			return (uint32_t) (depth2_chorus*100);
		case 5:
			return (uint32_t) (version_chorus);
		case 6:
			return (uint32_t) (is_active_chorus);		
	}
	return 0;
}


void Set_Parameters_Chorus(uint8_t delay, float rate1, float rate2, float depth1, float depth2, uint8_t version, uint8_t is_active)
{
	delay_chorus = delay;
	rate1_chorus = rate1;
	rate2_chorus = rate2;
	depth1_chorus = depth1;
	depth2_chorus = depth2;
	version_chorus = version;
	is_active_chorus = is_active;
}

void Display_Chorus_Window(void)
{
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);

	/* Przyciski */
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	// Rzad 1
	BSP_LCD_FillRect(RATE1_UP_BUTTON_XPOS,  PARAM1_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(RATE1_DOWN_BUTTON_XPOS, PARAM1_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(RATE2_UP_BUTTON_XPOS,  PARAM1_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(RATE2_DOWN_BUTTON_XPOS, PARAM1_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);	
	// Rzad 2
	BSP_LCD_FillRect(DEPTH_CH_UP_BUTTON_XPOS,  PARAM2_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DEPTH_CH_DOWN_BUTTON_XPOS, PARAM2_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DEPTH_UP_BUTTON_XPOS,  PARAM2_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DEPTH_DOWN_BUTTON_XPOS, PARAM2_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);	
	// Rzad 3
	BSP_LCD_FillRect(VERSION_BUTTON_XPOS,  PARAM3_CH_BUTTON_YPOS, VERSION_BUTTON_WIDTH, VERSION_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DELAY_CH_UP_BUTTON_XPOS,  PARAM3_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DELAY_CH_DOWN_BUTTON_XPOS, PARAM3_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	
	BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);

	/* Napisy */
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"CHORUS", LEFT_MODE);
	BSP_LCD_SetFont(&Font16);
	
	BSP_LCD_DisplayStringAt(RATE1_DOWN_BUTTON_XPOS+43, PARAM1_CH_BUTTON_YPOS-20, (uint8_t *)"RATE 1[Hz]:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(RATE2_DOWN_BUTTON_XPOS+43, PARAM1_CH_BUTTON_YPOS-20, (uint8_t *)"RATE 2[Hz]:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(DEPTH_CH_DOWN_BUTTON_XPOS+43, PARAM2_CH_BUTTON_YPOS-20, (uint8_t *)"DEPTH 1:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+43, PARAM2_CH_BUTTON_YPOS-20, (uint8_t *)"DEPTH 2:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS-8, PARAM3_CH_BUTTON_YPOS-20, (uint8_t *)"WERSJA:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(DELAY_CH_DOWN_BUTTON_XPOS+38, PARAM3_CH_BUTTON_YPOS-20, (uint8_t *)"DELAY[ms]:", LEFT_MODE);	
	
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	
	BSP_LCD_DisplayStringAt(RATE1_UP_BUTTON_XPOS+12,  PARAM1_CH_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(RATE1_DOWN_BUTTON_XPOS+12, PARAM1_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(RATE2_UP_BUTTON_XPOS+12,  PARAM1_CH_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(RATE2_DOWN_BUTTON_XPOS+12, PARAM1_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);			
	BSP_LCD_DisplayStringAt(DEPTH_CH_UP_BUTTON_XPOS+12,  PARAM2_CH_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(DEPTH_CH_DOWN_BUTTON_XPOS+12, PARAM2_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(DEPTH_UP_BUTTON_XPOS+12,  PARAM2_CH_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+12, PARAM2_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(DELAY_CH_UP_BUTTON_XPOS+12,  PARAM3_CH_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(DELAY_CH_DOWN_BUTTON_XPOS+12, PARAM3_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);
	
	if(version_chorus == SOI)
	{
		BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM3_CH_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"SOI", LEFT_MODE);	
	}else if(version_chorus== NOI)
	{
		BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM3_CH_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"NOI", LEFT_MODE);				
	}
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
	
	Display_Current_Parameters_Chorus();
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	if(is_active_chorus == NOT_ACTIVE)
	{	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
	}else if(is_active_chorus == ACTIVE)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
	}
}

void ChorusWindow_Touch_Detection(uint16_t x, uint16_t y)
{
	if((y >PARAM1_CH_BUTTON_YPOS) && (y < PARAM1_CH_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia parametru Rate 1 */	
		if((x >RATE1_UP_BUTTON_XPOS) && (x < RATE1_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(rate1_chorus<0.39f)
			{
				rate1_chorus +=0.01f;
				Display_Current_Parameters_Chorus();
			}
		/* Sprawdzenie przycisku zmnieszenia parametru Rate 1 */	
		}else if((x > RATE1_DOWN_BUTTON_XPOS) && (x < RATE1_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(rate1_chorus>0.01f)
			{
				rate1_chorus -=0.01f;
				Display_Current_Parameters_Chorus();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Rate 2 */		
		}	else if((x > RATE2_UP_BUTTON_XPOS) && (x < RATE2_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(rate2_chorus<0.39f)
			{
				rate2_chorus +=0.01f;
				Display_Current_Parameters_Chorus();
			} 
		/* Sprawdzenie przycisku zmniejszenia parametru Rate 2 */		
		}else if((x > RATE2_DOWN_BUTTON_XPOS) && (x < RATE2_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
		if(rate2_chorus>0.01f)
			{
				rate2_chorus -=0.01f;
				Display_Current_Parameters_Chorus();
			} 
		}
	}else if((y >PARAM2_CH_BUTTON_YPOS) && (y < PARAM2_CH_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia parametru Depth 1 */	
		if((x >DEPTH_CH_UP_BUTTON_XPOS) && (x < DEPTH_CH_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(depth1_chorus<0.99f)
			{
				depth1_chorus +=0.01f;
				Display_Current_Parameters_Chorus();
			}
		/* Sprawdzenie przycisku zmnieszenia parametru Depth 1 */	
		}else if((x > DEPTH_CH_DOWN_BUTTON_XPOS) && (x < DEPTH_CH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(depth1_chorus>0.01f)
			{
				depth1_chorus -=0.01f;
				Display_Current_Parameters_Chorus();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Depth 2 */		
		}	else if((x > DEPTH_UP_BUTTON_XPOS) && (x < DEPTH_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(depth2_chorus<0.99f)
			{
				depth2_chorus +=0.01f;
				Display_Current_Parameters_Chorus();
			} 
		/* Sprawdzenie przycisku zmniejszenia parametru Depth 2 */		
		}else if((x > DEPTH_DOWN_BUTTON_XPOS) && (x < DEPTH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(depth2_chorus>0.01f)
			{
				depth2_chorus -=0.01f;
				Display_Current_Parameters_Chorus();
			} 
		}
 }else if((y >PARAM3_CH_BUTTON_YPOS) && (y <PARAM3_CH_BUTTON_YPOS+ VERSION_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zmiany wersji efektu */	
		if((x > VERSION_BUTTON_XPOS) && (x < VERSION_BUTTON_XPOS +VERSION_BUTTON_WIDTH))
			{
				if(is_button_active==BUTTON_NOT_ACTIVE)
				{	
					BSP_LCD_SetFont(&Font16);
					BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					if(version_chorus == SOI)
					{
						BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM3_CH_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"NOI", LEFT_MODE);			
						version_chorus = NOI;
					}else if(version_chorus == NOI)
					{
						BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS+10, PARAM3_CH_BUTTON_YPOS+(VERSION_BUTTON_HEIGHT/3)-1, (uint8_t *)"SOI", LEFT_MODE);			
						version_chorus = SOI	;
					}
				}
			/* Sprawdzenie przycisku zwiekszenia parametru Delay */		
			}else if((x > DELAY_CH_UP_BUTTON_XPOS) && (x < DELAY_CH_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
			{
				if(delay_chorus<10)
				{
					delay_chorus +=1;
					Display_Current_Parameters_Chorus();
				} 
			/* Sprawdzenie przycisku zmniejszenia parametru Delay */							
			}else if((x > DELAY_CH_DOWN_BUTTON_XPOS) && (x < DELAY_CH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
			{
				if(delay_chorus>0)
				{
					delay_chorus -=1;
					Display_Current_Parameters_Chorus();
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
		Display_On_Off_Info_Chorus();
	}
}

void Display_Current_Parameters_Chorus(void)
{
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect(DELAY_CH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM3_CH_BUTTON_YPOS, PARAM_BUTTON_WIDTH+20, PARAM_BUTTON_HEIGHT);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetFont(&Font24);
	sprintf(rate1_str_chorus, "%.2f", rate1_chorus);
	BSP_LCD_DisplayStringAt(RATE1_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+20, PARAM1_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)rate1_str_chorus, LEFT_MODE);
	sprintf(rate2_str_chorus, "%.2f", rate2_chorus);
	BSP_LCD_DisplayStringAt(RATE2_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+20, PARAM1_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)rate2_str_chorus, LEFT_MODE);
	sprintf(depth1_str_chorus, "%.2f", depth1_chorus);
	BSP_LCD_DisplayStringAt(DEPTH_CH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+20, PARAM2_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)depth1_str_chorus, LEFT_MODE);
	sprintf(depth2_str_chorus, "%.2f", depth2_chorus);
	BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+20, PARAM2_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)depth2_str_chorus, LEFT_MODE);
	sprintf(delay_str_chorus, "%d", delay_chorus);
	BSP_LCD_DisplayStringAt(DELAY_CH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+35, PARAM3_CH_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)delay_str_chorus, LEFT_MODE);
}

void Display_On_Off_Info_Chorus(void)
{
	if(is_button_active==BUTTON_NOT_ACTIVE)
	{	
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		if(is_active_chorus == ACTIVE)
		{	
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
			is_active_chorus = NOT_ACTIVE;
			sprintf(current_info_text, "Wylaczono efekt Chorus");
		}else if(is_active_chorus == NOT_ACTIVE)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
			is_active_chorus= ACTIVE;	
			sprintf(current_info_text, "Wlaczono efekt Chorus");
		}
	} 
}

void Chorus(uint16_t *data_in, uint16_t* data_out, uint32_t count)
{
	if(is_active_chorus == ACTIVE)
	{
		uint16_t max_delay = (delay_chorus * 44100)/ 1000;
		uint16_t chorus_period1 = 44100 / rate1_chorus;
		uint16_t chorus_period2 = 44100 / rate2_chorus;
		uint16_t n;
		uint16_t delay_samples1;
		uint16_t delay_samples2;
		static uint16_t chorus_n1 = 0;
		static uint16_t chorus_n2 = 0;
		float frequency_change1 = (float)(rate1_chorus)/44100;
		float frequency_change2 = (float)(rate2_chorus)/44100;
		int16_t curr_sample;
		int16_t prev_sample1;
		int16_t prev_sample2;		
		int16_t out_sample;
		

		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
		{								
			curr_sample = (int16_t)data_out[n];
			delay_samples1 = 1 + (max_delay/2)*(1-arm_cos_f32(2* PI* frequency_change1*chorus_n1));
			delay_samples2 = 1 + (max_delay/2)*(1-arm_cos_f32(2* PI* frequency_change2*chorus_n2));
			
			if(version_chorus == SOI)
			{
				prev_sample1 = data_in[(n + AUDIO_BUFFER_SIZE - delay_samples1) % AUDIO_BUFFER_SIZE];
				prev_sample2 = data_in[(n + AUDIO_BUFFER_SIZE - delay_samples2) % AUDIO_BUFFER_SIZE];
			}else
			{
				prev_sample1 = data_out[(n + AUDIO_BUFFER_SIZE - delay_samples1) % AUDIO_BUFFER_SIZE];
				prev_sample2 = data_out[(n + AUDIO_BUFFER_SIZE - delay_samples2) % AUDIO_BUFFER_SIZE];
			}
			out_sample = (curr_sample + depth1_chorus*prev_sample1 +depth2_chorus*prev_sample2)/(1+depth1_chorus + depth2_chorus);
			data_out[n] = (uint16_t) (out_sample);

			chorus_n1 = (chorus_n1 + chorus_period1 + 1) % chorus_period1;
			chorus_n2 = (chorus_n2 + chorus_period2 + 1) % chorus_period2;
		}
	}
}

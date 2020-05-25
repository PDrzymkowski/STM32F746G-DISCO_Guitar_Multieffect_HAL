/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : tremolo.c
  * @brief         : Efekt tremolo, modulacja amplitudowa sygnalu
  ******************************************************************************
  */
/* USER CODE END Header */

#include "tremolo.h"

static uint8_t rate_tremolo = 5;
static float depth_tremolo = 0.5f;
static uint8_t shape_tremolo = SIN;
static uint8_t is_active_tremolo = NOT_ACTIVE;
static char rate_str_tremolo[3];
static char depth_str_tremolo[3];
extern uint8_t is_button_active;
extern char current_info_text[40];


uint32_t Get_Parameter_Tremolo(uint8_t parameter)
{
	switch(parameter)
	{
		case 0:
			return (uint32_t) (rate_tremolo);
		case 1:
			return (uint32_t) (depth_tremolo*100);
		case 2:
			return (uint32_t) (shape_tremolo);
		case 3:
			return (uint32_t) (is_active_tremolo);
	}
	return 0;
}

void Set_Parameters_Tremolo(uint8_t rate, float depth, uint8_t shape, uint8_t is_active)
{
	rate_tremolo = rate;
	depth_tremolo = depth;
	shape_tremolo = shape;
	is_active_tremolo = is_active;
}

void Display_Tremolo_Window(void)
{
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);

	/* Przyciski */
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(RATE1_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(RATE1_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DEPTH_UP_BUTTON_XPOS,  PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(DEPTH_DOWN_BUTTON_XPOS, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(SHAPE_BUTTON_XPOS,  PARAM2_BUTTON_YPOS, SHAPE_BUTTON_WIDTH, SHAPE_BUTTON_HEIGHT);
	BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);

	/* Napisy */
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"TREMOLO", LEFT_MODE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(RATE1_DOWN_BUTTON_XPOS+38, PARAM1_BUTTON_YPOS-20, (uint8_t *)"RATE[Hz]:", LEFT_MODE);	
	BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+60, PARAM1_BUTTON_YPOS-20, (uint8_t *)"DEPTH:", LEFT_MODE);		
	BSP_LCD_DisplayStringAt(SHAPE_BUTTON_XPOS-15, PARAM2_BUTTON_YPOS-20, (uint8_t *)"KSZTALT:", LEFT_MODE);		
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(RATE1_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(RATE1_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);
	BSP_LCD_DisplayStringAt(DEPTH_UP_BUTTON_XPOS+12,  PARAM1_BUTTON_YPOS+( PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+12, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)-1, (uint8_t *)"-", LEFT_MODE);	
	if(shape_tremolo == SIN)
	{
		BSP_LCD_DisplayStringAt(SHAPE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(SHAPE_BUTTON_HEIGHT/3)-1, (uint8_t *)"SIN", LEFT_MODE);	
	}else if(shape_tremolo == TRI)
	{
		BSP_LCD_DisplayStringAt(SHAPE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(SHAPE_BUTTON_HEIGHT/3)-1, (uint8_t *)"TRI", LEFT_MODE);				
	}else if(shape_tremolo ==SQU)
	{
		BSP_LCD_DisplayStringAt(SHAPE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(SHAPE_BUTTON_HEIGHT/3)-1, (uint8_t *)"SQU", LEFT_MODE);				
	}
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
	
	Display_Current_Parameters_Tremolo();
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	if(is_active_tremolo == NOT_ACTIVE)
	{	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
	}else if(is_active_tremolo == ACTIVE)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
	}	
}

void TremoloWindow_Touch_Detection(uint16_t x, uint16_t y)
{
	if((y >PARAM1_BUTTON_YPOS) && (y <PARAM1_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia parametru Rate */	
		if((x > RATE1_UP_BUTTON_XPOS) && (x < RATE1_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(rate_tremolo<20)
			{
				rate_tremolo += 1;
				Display_Current_Parameters_Tremolo();
			}
		/* Sprawdzenie przycisku zmnieszenia parametru Rate */	
		}else if((x > RATE1_DOWN_BUTTON_XPOS) && (x < RATE1_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
		if(rate_tremolo>0)
			{
				rate_tremolo -= 1;
				Display_Current_Parameters_Tremolo();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Depth */		
		}else if((x > DEPTH_UP_BUTTON_XPOS) && (x < DEPTH_UP_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(depth_tremolo<0.99f)
			{
				depth_tremolo += 0.01f;
				Display_Current_Parameters_Tremolo();
			} 
		/* Sprawdzenie przycisku zmniejszenia parametru Depth */		
		}else if((x > DEPTH_DOWN_BUTTON_XPOS) && (x < DEPTH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(depth_tremolo>0.01f)
			{
				depth_tremolo -= 0.01f;
				Display_Current_Parameters_Tremolo();
			} 
		}
	}else if((y > PARAM2_BUTTON_YPOS) && (y < PARAM2_BUTTON_YPOS + SHAPE_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zmiany ksztaltu sygnalu modulujacego */	
		if((x > SHAPE_BUTTON_XPOS) && (x < SHAPE_BUTTON_XPOS + SHAPE_BUTTON_WIDTH))
		{
			if(is_button_active==BUTTON_NOT_ACTIVE)
			{	
				BSP_LCD_SetFont(&Font16);
				BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				if(shape_tremolo == SIN)
				{
					BSP_LCD_DisplayStringAt(SHAPE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(SHAPE_BUTTON_HEIGHT/3)-1, (uint8_t *)"TRI", LEFT_MODE);			
					shape_tremolo = TRI;
				}else if(shape_tremolo == TRI)
				{
					BSP_LCD_DisplayStringAt(SHAPE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(SHAPE_BUTTON_HEIGHT/3)-1, (uint8_t *)"SQU", LEFT_MODE);			
					shape_tremolo = SQU;
				}else if(shape_tremolo == SQU)
				{
					BSP_LCD_DisplayStringAt(SHAPE_BUTTON_XPOS+10, PARAM2_BUTTON_YPOS+(SHAPE_BUTTON_HEIGHT/3)-1, (uint8_t *)"SIN", LEFT_MODE);			
					shape_tremolo = SIN;
				}
	 /* Sprawdzenie przycisku powrotu do menu */	
			}
		}
	}else if((y > BACK_BUTTON_YPOS) && (y < BACK_BUTTON_YPOS+ BACK_BUTTON_HEIGHT) &&
							    	(x > BACK_BUTTON_XPOS) && (x < BACK_BUTTON_XPOS+ BACK_BUTTON_WIDTH))
	{
		Display_MainWindow();
	/* Sprawdzenie przycisku wlaczenia efektu */					
	}else if((y > ON_OFF_BUTTON_YPOS) && (y < ON_OFF_BUTTON_YPOS + ON_OFF_BUTTON_HEIGHT) &&
								(x > ON_OFF_BUTTON_XPOS) && (x < ON_OFF_BUTTON_XPOS + ON_OFF_BUTTON_WIDTH))
	{
		Display_On_Off_Info_Tremolo();
	}
}


void Display_Current_Parameters_Tremolo(void)
{
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect(RATE1_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS, PARAM_BUTTON_WIDTH+20, PARAM_BUTTON_HEIGHT);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetFont(&Font24);
	sprintf(rate_str_tremolo, "%d", rate_tremolo);
	BSP_LCD_DisplayStringAt(RATE1_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+30, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)rate_str_tremolo, LEFT_MODE);
	sprintf(depth_str_tremolo, "%.2f", depth_tremolo);
	BSP_LCD_DisplayStringAt(DEPTH_DOWN_BUTTON_XPOS+PARAM_BUTTON_WIDTH+25, PARAM1_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/2)-8, (uint8_t *)depth_str_tremolo, LEFT_MODE);
}

float Square_Signal(uint16_t x, uint16_t period)
{
	if(x <= period/2)
		return 1;
	else
		return -1;
}


float Triangle_Signal(uint16_t x, uint16_t period)
{
	if(x <= period/2)
		return ( ((float)(x) *4/period) - 1 );
	else
		return ( 3 - ((float)(x) *4/period) ); 
	
}

void Display_On_Off_Info_Tremolo(void)
{
	if(is_button_active==BUTTON_NOT_ACTIVE)
	{	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	if(is_active_tremolo == ACTIVE)
	{	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WYLACZONY", LEFT_MODE);
		is_active_tremolo = NOT_ACTIVE;
		sprintf(current_info_text, "Wylaczono efekt Tremolo");
	}else if(is_active_tremolo == NOT_ACTIVE)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
		is_active_tremolo= ACTIVE;	
		sprintf(current_info_text, "Wlaczono efekt Tremolo");
		}
	} 
}

void Tremolo(uint16_t* data_out, uint32_t count)
{
	if(is_active_tremolo == ACTIVE)
	{
		float frequency_mod = (float)(rate_tremolo)/44100;
		uint16_t tremolo_period = 44100 / rate_tremolo;
		static uint16_t tremolo_n = 0;
		uint16_t n;
		int16_t curr_sample;
		int16_t out_sample;
	
		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
		{
			curr_sample = (int16_t) data_out[n];
			if(shape_tremolo == SIN)
			{
				out_sample = (curr_sample *(1 + depth_tremolo*arm_cos_f32(2*PI*frequency_mod*tremolo_n)));		
			}else if(shape_tremolo == SQU) 
			{
				out_sample = (curr_sample *(1 + depth_tremolo*Square_Signal(tremolo_n, tremolo_period)));			
			}else if(shape_tremolo == TRI)
			{
				out_sample = (curr_sample *(1 + depth_tremolo*Triangle_Signal(tremolo_n, tremolo_period)));		
			}
			data_out[n] = (uint16_t) out_sample;				
			tremolo_n = (tremolo_n + tremolo_period + 1) % tremolo_period;
		}
	}
}

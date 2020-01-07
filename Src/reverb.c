/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : reverb.c
  * @brief         : Efekt reverb, dodanie poglosu do sygnalu
  ******************************************************************************
  */
/* USER CODE END Header */

#include "reverb.h"

uint8_t room_size_reverb = 50;
float depth_reverb = 0.5;
uint8_t pre_delay1_reverb = 10;
uint8_t pre_delay2_reverb = 10;
uint8_t is_active_reverb = NOT_ACTIVE;
char room_size_str_reverb[3];
char depth_str_reverb[3];
char pre_delay1_str_reverb[3];
char pre_delay2_str_reverb[3];
extern uint8_t is_button_active;


uint32_t Get_Parameter_Reverb(uint8_t parameter)
{
	switch(parameter)
	{
		case 0:
			return (uint32_t) (room_size_reverb);
		case 1:
			return (uint32_t) (depth_reverb*100);
		case 2:
			return (uint32_t) (pre_delay1_reverb);
		case 3:
			return (uint32_t) (pre_delay2_reverb);
		case 4:
			return (uint32_t) (is_active_reverb);
	}
	return 0;
}

void Set_Parameters_Reverb(uint8_t room_size, float depth, uint8_t pre_delay1,uint8_t pre_delay2, uint8_t is_active)
{
	room_size_reverb = room_size;
	depth_reverb = depth;
	pre_delay1_reverb = pre_delay1;
	pre_delay2_reverb = pre_delay2;
	is_active_reverb = is_active;
}

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
	
	Display_Current_Parameters_Reverb();
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
				room_size_reverb += 1;
				Display_Current_Parameters_Reverb();
			}
		/* Sprawdzenie przycisku zmnieszenia parametru Room Size */	
		}else if((x > ROOM_SIZE_DOWN_BUTTON_XPOS) && (x < ROOM_SIZE_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(room_size_reverb>0)
			{
				room_size_reverb -= 1;
				Display_Current_Parameters_Reverb();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Depth */		
		}else if((x > DEPTH_UP_BUTTON_XPOS) && (x < DEPTH_UP_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(depth_reverb<0.99f)
			{
				depth_reverb += 0.01f;
				Display_Current_Parameters_Reverb();
			} 
		/* Sprawdzenie przycisku zmniejszenia parametru Depth */		
		}else if((x > GAIN_DOWN_BUTTON_XPOS) && (x < GAIN_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(depth_reverb>0.01f)
			{
				depth_reverb -= 0.01f;
				Display_Current_Parameters_Reverb();
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
				Display_Current_Parameters_Reverb();
			}
		/* Sprawdzenie przycisku zmnieszenia parametru Pre Delay 1 */	
		}else if((x > PRE_DELAY1_DOWN_BUTTON_XPOS) && (x < PRE_DELAY1_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
		if(pre_delay1_reverb>0)
			{
				pre_delay1_reverb -=1;
				Display_Current_Parameters_Reverb();
			}
		/* Sprawdzenie przycisku zwiekszenia parametru Pre Delay 2 */		
		}	else if((x > PRE_DELAY2_UP_BUTTON_XPOS) && (x < PRE_DELAY2_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
		{
			if(pre_delay2_reverb<20)
			{
				pre_delay2_reverb +=1;
				Display_Current_Parameters_Reverb();
			} 
		/* Sprawdzenie przycisku zmniejszenia parametru Pre Delay 2 */		
		}else if((x > PRE_DELAY2_DOWN_BUTTON_XPOS) && (x < PRE_DELAY2_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(pre_delay2_reverb>0)
			{
				pre_delay2_reverb -=1;
				Display_Current_Parameters_Reverb();
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
		Display_On_Off_Info_Reverb();
	}
}

void Display_Current_Parameters_Reverb(void)
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

void Reverb(uint16_t *data_in, uint16_t* data_out, uint32_t count)
{
	if(is_active_reverb == ACTIVE)
	{
		uint16_t n;
		uint16_t D_n[6];	
		float a_n[2];
		
		int16_t out_sample;
		static int16_t comb_1_out[COMB_1_OUT_MAX_SIZE] = {0};
		static int16_t comb_2_out[COMB_2_OUT_MAX_SIZE] = {0};
		static int16_t comb_3_out[COMB_3_OUT_MAX_SIZE] = {0};
		static int16_t comb_4_out[COMB_4_OUT_MAX_SIZE] = {0};
		static int16_t comb_sect_out[COMB_SECT_OUT_MAX_SIZE] = {0};
		static int16_t all_pass_out[ALL_PASS_OUT_MAX_SIZE] = {0};
		
		static uint16_t comb_1_out_n = 0;
		static uint16_t comb_2_out_n = 0;
		static uint16_t comb_3_out_n = 0;
		static uint16_t comb_4_out_n = 0;
		static uint16_t comb_sect_out_n = 0;
		static uint16_t all_pass_out_n = 0;
	
		D_n[0] = (room_size_reverb*44100)/1000;
		D_n[1] = (room_size_reverb*44100*1.17)/1000;
		D_n[2] = (room_size_reverb*44100*1.33)/1000;
		D_n[3] = (room_size_reverb*44100*1.5)/1000;
		D_n[4] = (pre_delay1_reverb*44100)/1000;
		D_n[5] = (pre_delay2_reverb*44100)/1000;
	
		a_n[0] = depth_reverb;
		a_n[1] = 0.7;
	
		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++)
		{								
			comb_1_out[comb_1_out_n] = ((int16_t)data_out[n] + a_n[0]*comb_1_out[(comb_1_out_n + COMB_1_OUT_MAX_SIZE - D_n[0]) % COMB_1_OUT_MAX_SIZE])/(1+a_n[0]);
			comb_2_out[comb_2_out_n] = ((int16_t)data_out[n] + a_n[0]*comb_2_out[(comb_2_out_n + COMB_2_OUT_MAX_SIZE - D_n[1]) % COMB_2_OUT_MAX_SIZE])/(1+a_n[0]);
			comb_3_out[comb_3_out_n] = ((int16_t)data_out[n] + a_n[0]*comb_3_out[(comb_3_out_n + COMB_3_OUT_MAX_SIZE - D_n[2]) % COMB_3_OUT_MAX_SIZE])/(1+a_n[0]);
			comb_4_out[comb_1_out_n] = ((int16_t)data_out[n] + a_n[0]*comb_4_out[(comb_4_out_n + COMB_4_OUT_MAX_SIZE - D_n[3]) % COMB_4_OUT_MAX_SIZE])/(1+a_n[0]); 
	
			comb_sect_out[comb_sect_out_n] = (comb_1_out[comb_1_out_n] + 	comb_2_out[comb_2_out_n] + 	comb_3_out[comb_3_out_n] + 	comb_4_out[comb_4_out_n]);
			
			all_pass_out[all_pass_out_n] =(((-a_n[1]) * comb_sect_out[comb_sect_out_n]) 
														+  comb_sect_out[(comb_sect_out_n + COMB_SECT_OUT_MAX_SIZE - D_n[4]) % COMB_SECT_OUT_MAX_SIZE] 
														+	all_pass_out[(all_pass_out_n + ALL_PASS_OUT_MAX_SIZE - D_n[4]) % ALL_PASS_OUT_MAX_SIZE])/(1+a_n[1] + a_n[1]); 			
			
			out_sample = (((-a_n[1]) * all_pass_out[all_pass_out_n])
														+ all_pass_out[(all_pass_out_n + ALL_PASS_OUT_MAX_SIZE - D_n[5]) % ALL_PASS_OUT_MAX_SIZE]
														+	(int16_t)(data_out[(n + AUDIO_BUFFER_SIZE - D_n[5]) % AUDIO_BUFFER_SIZE]))/(1+a_n[1]+a_n[1]); 	
		
			out_sample = out_sample * 2;
			data_out[n] = (uint16_t) out_sample;
			
			comb_1_out_n = (comb_1_out_n +  1) % COMB_1_OUT_MAX_SIZE;
			comb_2_out_n = (comb_2_out_n +  1) % COMB_2_OUT_MAX_SIZE;
			comb_3_out_n = (comb_3_out_n +  1) % COMB_3_OUT_MAX_SIZE;
			comb_4_out_n = (comb_4_out_n + 1) % COMB_4_OUT_MAX_SIZE;
			comb_sect_out_n = (comb_sect_out_n + 1) % COMB_SECT_OUT_MAX_SIZE;
			all_pass_out_n = (all_pass_out_n + 1) % ALL_PASS_OUT_MAX_SIZE;
		}
	}
}

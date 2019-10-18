#include "chorus.h"

uint8_t delay_chorus = 5;
char delay_str_chorus[3];
double rate1_chorus = 0.2;
char  rate1_str_chorus[3];
double rate2_chorus = 0.2;
char rate2_str_chorus[3];
double depth1_chorus = 0.5;
char depth1_str_chorus[3];
double depth2_chorus = 0.5;
char depth2_str_chorus[3];
uint8_t version_chorus = SOI;
uint8_t is_active_chorus = NOT_ACTIVE;
extern uint8_t is_button_active;


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
		BSP_LCD_DisplayStringAt(VERSION_BUTTON_XPOS-10, PARAM3_CH_BUTTON_YPOS-20, (uint8_t *)"WERSJA:", LEFT_MODE);	
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
		
		Display_Current_Parametres_Chorus();
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
					if(rate1_chorus<0.40)
					{
						rate1_chorus +=0.01;
						Display_Current_Parametres_Chorus();
					}
				/* Sprawdzenie przycisku zmnieszenia parametru Rate 1 */	
				}else if((x > RATE1_DOWN_BUTTON_XPOS) && (x < RATE1_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(rate1_chorus>0.01)
					{
						rate1_chorus -=0.01;
						Display_Current_Parametres_Chorus();
					}
				/* Sprawdzenie przycisku zwiekszenia parametru Rate 2 */		
				}	else if((x > RATE2_UP_BUTTON_XPOS) && (x < RATE2_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(rate2_chorus<0.40)
					{
						rate2_chorus +=0.01;
						Display_Current_Parametres_Chorus();
					} 
				/* Sprawdzenie przycisku zmniejszenia parametru Rate 2 */		
				}else if((x > RATE2_DOWN_BUTTON_XPOS) && (x < RATE2_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(rate2_chorus>0.01)
					{
						rate2_chorus -=0.01;
						Display_Current_Parametres_Chorus();
					} 
				}
		 }else if((y >PARAM2_CH_BUTTON_YPOS) && (y < PARAM2_CH_BUTTON_YPOS+ PARAM_BUTTON_HEIGHT))
		{
				/* Sprawdzenie przycisku zwiekszenia parametru Depth 1 */	
				if((x >DEPTH_CH_UP_BUTTON_XPOS) && (x < DEPTH_CH_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(depth1_chorus<1.00)
					{
						depth1_chorus +=0.01;
						Display_Current_Parametres_Chorus();
					}
				/* Sprawdzenie przycisku zmnieszenia parametru Depth 1 */	
				}else if((x > DEPTH_CH_DOWN_BUTTON_XPOS) && (x < DEPTH_CH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(depth1_chorus>0.01)
					{
						depth1_chorus -=0.01;
						Display_Current_Parametres_Chorus();
					}
				/* Sprawdzenie przycisku zwiekszenia parametru Depth 2 */		
				}	else if((x > DEPTH_UP_BUTTON_XPOS) && (x < DEPTH_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(depth2_chorus<1.00)
					{
						depth2_chorus +=0.01;
						Display_Current_Parametres_Chorus();
					} 
				/* Sprawdzenie przycisku zmniejszenia parametru Depth 2 */		
				}else if((x > DEPTH_DOWN_BUTTON_XPOS) && (x < DEPTH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(depth2_chorus>0.01)
					{
						depth2_chorus -=0.01;
						Display_Current_Parametres_Chorus();
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
						Display_Current_Parametres_Chorus();
					} 
				/* Sprawdzenie przycisku zmniejszenia parametru Delay */							
				}else if((x > DELAY_CH_DOWN_BUTTON_XPOS) && (x < DELAY_CH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
						if(delay_chorus>0)
					{
						delay_chorus -=1;
						Display_Current_Parametres_Chorus();
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

void Display_Current_Parametres_Chorus(void)
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
		}else if(is_active_chorus == NOT_ACTIVE)
		{
				BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
				BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
				is_active_chorus= ACTIVE;	
		}
	} 
}

void Chorus(void)
{
	if(is_active_chorus == ACTIVE)
	{
		
	}
}

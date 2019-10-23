#include "tremolo.h"

uint8_t rate_tremolo = 5;
char rate_str_tremolo[3];
double depth_tremolo = 0.5;
char depth_str_tremolo[3];
uint8_t shape_tremolo = SIN;
uint8_t is_active_tremolo = NOT_ACTIVE;
extern uint8_t is_button_active;


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
		
		Display_Current_Parametres_Tremolo();
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
						rate_tremolo +=1;
						Display_Current_Parametres_Tremolo();
					}
				/* Sprawdzenie przycisku zmnieszenia parametru Rate */	
				}else if((x > RATE1_DOWN_BUTTON_XPOS) && (x < RATE1_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(rate_tremolo>0)
					{
						rate_tremolo -=1;
						Display_Current_Parametres_Tremolo();
					}
				/* Sprawdzenie przycisku zwiekszenia parametru Depth */		
				}	else if((x > DEPTH_UP_BUTTON_XPOS) && (x < DEPTH_UP_BUTTON_XPOS +PARAM_BUTTON_WIDTH))
				{
					if(depth_tremolo<1)
					{
						depth_tremolo +=0.01;
						Display_Current_Parametres_Tremolo();
					} 
				/* Sprawdzenie przycisku zmniejszenia parametru Depth */		
				}else if((x > DEPTH_DOWN_BUTTON_XPOS) && (x < DEPTH_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
				{
				if(depth_tremolo>0.01)
					{
						depth_tremolo -=0.01;
						Display_Current_Parametres_Tremolo();
					} 
					
				}
		 }else if((y >PARAM2_BUTTON_YPOS) && (y <PARAM2_BUTTON_YPOS+ SHAPE_BUTTON_HEIGHT))
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


void Display_Current_Parametres_Tremolo(void)
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
		}else if(is_active_tremolo == NOT_ACTIVE)
		{
				BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS, ON_OFF_BUTTON_YPOS, ON_OFF_BUTTON_WIDTH, ON_OFF_BUTTON_HEIGHT);
				BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
				BSP_LCD_FillRect(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), 70, 20);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(ON_OFF_BUTTON_XPOS-80, ON_OFF_BUTTON_YPOS+(ON_OFF_BUTTON_HEIGHT/3), (uint8_t *)"WLACZONY", LEFT_MODE);
				is_active_tremolo= ACTIVE;	
		}
	} 
}

void Tremolo(uint16_t *data)
{
		if(is_active_tremolo == ACTIVE)
	{
		
	}
}

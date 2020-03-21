/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : volume.c
  * @brief         : Obsluga zmiany glosnosci sygnalu
  ******************************************************************************
  */
/* USER CODE END Header */

#include "volume.h"

static uint8_t current_volume = 70;
static uint8_t current_volume_temp;
static char current_volume_str[3];
extern char current_info_text[40];


uint32_t Get_Parameter_Volume(void)
{
	return (uint32_t) current_volume;
}

void Set_Parameters_Volume(uint8_t volume)
{
	current_volume = volume;
}


void Display_Current_Volume(void)
{
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_FillRect(VOLUME_DOWN_BUTTON_XPOS+VOLUME_BUTTON_WIDTH+25, VOLUME_BUTTON_YPOS, VOLUME_BUTTON_WIDTH+20, VOLUME_BUTTON_HEIGHT);

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetFont(&Font24);
	sprintf(current_volume_str, "%d", current_volume_temp);
	BSP_LCD_DisplayStringAt(VOLUME_DOWN_BUTTON_XPOS+VOLUME_BUTTON_WIDTH+30, VOLUME_BUTTON_YPOS+(VOLUME_BUTTON_HEIGHT/2)-8, (uint8_t *)current_volume_str, LEFT_MODE);
}


void Display_Volume_Window(void)
{
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	/* Przyciski */
	BSP_LCD_FillRect(VOLUME_UP_BUTTON_XPOS, VOLUME_BUTTON_YPOS, VOLUME_BUTTON_WIDTH, VOLUME_BUTTON_HEIGHT);
	BSP_LCD_FillRect(VOLUME_DOWN_BUTTON_XPOS, VOLUME_BUTTON_YPOS, VOLUME_BUTTON_WIDTH, VOLUME_BUTTON_HEIGHT);
	BSP_LCD_FillRect(VOLUME_10_UP_BUTTON_XPOS, VOLUME_10_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(VOLUME_10_DOWN_BUTTON_XPOS, VOLUME_10_BUTTON_YPOS, PARAM_BUTTON_WIDTH, PARAM_BUTTON_HEIGHT);
	BSP_LCD_FillRect(APPLY_BUTTON_XPOS, APPLY_BUTTON_YPOS, APPLY_BUTTON_WIDTH, APPLY_BUTTON_HEIGHT);	
	BSP_LCD_FillRect(BACK_BUTTON_XPOS, BACK_BUTTON_YPOS, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);	

	/* Napisy */
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(TITLE_XPOS, TITLE_YPOS, (uint8_t *)"GLOSNOSC", LEFT_MODE);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(100, 50, (uint8_t *)"Zmiana glosnosci wyjsciowej w zakresie 0% - 100%", LEFT_MODE);
	BSP_LCD_DisplayStringAt(100, 65, (uint8_t *)"zachodzi przy uzyciu przyciskow '+' i '-' oraz po ", LEFT_MODE);
	BSP_LCD_DisplayStringAt(165, 80, (uint8_t *)"wcisnieciu przycisku 'ZASTOSUJ'", LEFT_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(BACK_BUTTON_XPOS+12, BACK_BUTTON_YPOS+(BACK_BUTTON_HEIGHT/2)-3, (uint8_t *)"POWROT", LEFT_MODE);
	
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(VOLUME_UP_BUTTON_XPOS+16, VOLUME_BUTTON_YPOS+(VOLUME_BUTTON_HEIGHT/3)-3, (uint8_t *)"+", LEFT_MODE);
	BSP_LCD_DisplayStringAt(VOLUME_DOWN_BUTTON_XPOS+16, VOLUME_BUTTON_YPOS+(VOLUME_BUTTON_HEIGHT/3)-3, (uint8_t *)"-", LEFT_MODE);
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(VOLUME_10_UP_BUTTON_XPOS+5, VOLUME_10_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)+1, (uint8_t *)"+10", LEFT_MODE);
	BSP_LCD_DisplayStringAt(VOLUME_10_DOWN_BUTTON_XPOS+5, VOLUME_10_BUTTON_YPOS+(PARAM_BUTTON_HEIGHT/3)+1, (uint8_t *)"-10", LEFT_MODE);
	
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(APPLY_BUTTON_XPOS+7, APPLY_BUTTON_YPOS+(APPLY_BUTTON_HEIGHT/3), (uint8_t *)"ZASTOSUJ", LEFT_MODE);

	current_volume_temp = current_volume;
	
	Display_Current_Volume();
}

void VolumeWindow_Touch_Detection(uint16_t x, uint16_t y)
{
	if((y > VOLUME_BUTTON_YPOS) && (y < VOLUME_BUTTON_YPOS + VOLUME_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zmniejszenia glosnosci */	
		if((x > VOLUME_UP_BUTTON_XPOS) && (x < VOLUME_UP_BUTTON_XPOS + VOLUME_BUTTON_WIDTH))
		{
			if(current_volume_temp < 100)
			{
				current_volume_temp += 1;
				Display_Current_Volume();
			}
		/* Sprawdzenie przycisku zwiekszenia glosnosci */	
		}else if((x > VOLUME_DOWN_BUTTON_XPOS) && (x < VOLUME_DOWN_BUTTON_XPOS + VOLUME_BUTTON_WIDTH))
		{
			if(current_volume_temp > 0)
			{
				current_volume_temp -= 1;
				Display_Current_Volume();
			}
		}
	}else if((y > VOLUME_10_BUTTON_YPOS) && (y < VOLUME_10_BUTTON_YPOS + VOLUME_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku zwiekszenia glosnosci o 10 */	
		if((x > VOLUME_10_UP_BUTTON_XPOS) && (x < VOLUME_10_UP_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(current_volume_temp < 91)
			{
				current_volume_temp += 10;
				Display_Current_Volume();
			}
		/* Sprawdzenie przycisku zmniejszenia glosnosci o 10 */	
		}else if((x > VOLUME_10_DOWN_BUTTON_XPOS) && (x < VOLUME_10_DOWN_BUTTON_XPOS + PARAM_BUTTON_WIDTH))
		{
			if(current_volume_temp > 9)
			{
				current_volume_temp -= 10;
				Display_Current_Volume();
			}
		}
	/* Sprawdzenie przycisku powrotu do menu */
	}else if((y > BACK_BUTTON_YPOS) && (y < BACK_BUTTON_YPOS + BACK_BUTTON_HEIGHT) &&
									(x > BACK_BUTTON_XPOS) && (x < BACK_BUTTON_XPOS + BACK_BUTTON_WIDTH))
	{
		Display_MainWindow();
	/* Sprawdzenie przycisku zastosowania zmian */		
	}else if((y > APPLY_BUTTON_YPOS) && (y < APPLY_BUTTON_YPOS + APPLY_BUTTON_HEIGHT) &&
									(x > APPLY_BUTTON_XPOS) && (x < APPLY_BUTTON_XPOS + APPLY_BUTTON_WIDTH))
	{
		current_volume = current_volume_temp;
		if(BSP_AUDIO_OUT_SetVolume(current_volume) == AUDIO_OK)
		{
		 BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		 BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		 BSP_LCD_SetFont(&Font24);
		 BSP_LCD_DisplayStringAt(APPLY_BUTTON_XPOS+APPLY_BUTTON_WIDTH+20, APPLY_BUTTON_YPOS+(APPLY_BUTTON_HEIGHT/3), (uint8_t *)"OK", LEFT_MODE);
		}else
		{
		 BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		 BSP_LCD_SetTextColor(LCD_COLOR_RED);
		 BSP_LCD_SetFont(&Font24);
		 BSP_LCD_DisplayStringAt(APPLY_BUTTON_XPOS+APPLY_BUTTON_WIDTH+20, APPLY_BUTTON_YPOS+(APPLY_BUTTON_HEIGHT/3), (uint8_t *)"ERROR", LEFT_MODE);
		}			
		sprintf(current_info_text, "Zmieniono glosnosc na wartosc: %d%%", current_volume);
	}
}

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "volume.h"
#include "overdrive.h"
#include "chorus.h"
#include "reverb.h"
#include "delay.h"
#include "pitch_shifter.h"
#include "tremolo.h"
#include "flanger.h"
#include "tim.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;

typedef enum
{
	MAIN_WINDOW = 0,
	VOLUME_WINDOW = 1,
	REVERB_WINDOW = 2,
	OVERDRIVE_WINDOW = 3,
	DELAY_WINDOW = 4,
	FLANGER_WINDOW = 5,
	CHORUS_WINDOW = 6,
	TREMOLO_WINDOW = 7,
	PITCHSHIFTER_WINDOW = 8,
	START_WINDOW = 9,
}CURRENT_WINDOW_StateTypeDef;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

TIM_HandleTypeDef htim10;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */
void Display_StartWindow(uint8_t is_blinking);
void StartWindow_Touch_Detection(void);
void Multieffect(void);
void LCD_Multieffect_Init(void);
void Current_Window_Select(void);
void MainWindow_Touch_Detection(void);
void Signal_Processing(uint16_t *data_in, uint16_t *data_out, uint32_t count);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
TS_StateTypeDef 			ts;
char xTouchStr[10];
uint16_t x, y;
uint16_t n, m;
uint8_t  audio_rec_buffer_state;
uint8_t current_window;
uint32_t count = 0;
uint8_t is_codec_initialized = AUDIO_ERROR;

uint8_t second_counter = 0;

int16_t sample1;
uint32_t sample2;
uint32_t sample1x;
uint32_t sample2x;

char button_names[8][14] = {
			
			"Reverb",
			"Overdrive",
			"Delay",
			"Glosnosc",
			"Flanger",
			"Chorus",
			"Tremolo",	
			"Pitch Shifter"
};


uint16_t data_in[AUDIO_BUFFER_SIZE];
uint16_t data_out[AUDIO_BUFFER_SIZE];
uint16_t data_out_processed[2*AUDIO_BLOCK_SIZE];
uint8_t is_button_active= BUTTON_NOT_ACTIVE;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
							/*Enable the CPU Cache */
							/*Enable I-Cache and D-Cache */
							SCB_EnableICache();
							SCB_EnableDCache();
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */

	BSP_SDRAM_Init(); /* Initializes the SDRAM device */
	__HAL_RCC_CRC_CLK_ENABLE(); /* Enable the CRC Module */

	BSP_LED_Init(LED1);
/* Configure the User Button in GPIO Mode */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	LCD_Multieffect_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		

    /* USER CODE BEGIN 3 */
				Multieffect();
  }
	
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 20000;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 1000;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


void Display_StartWindow(uint8_t is_blinking) 
{
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(GREETING_TITLE_XPOS, GREETING_TITLE_YPOS, (uint8_t *)"MULTIEFEKT GITAROWY", LEFT_MODE);
	BSP_LCD_SetFont(&Font12);
	if(is_blinking == 1)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_DisplayStringAt(GREETING_TITLE_XPOS+85, GREETING_TITLE_YPOS+80, (uint8_t *)"KLIKNIJ ABY URUCHOMIC:", LEFT_MODE);
	}
	else
	{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(GREETING_TITLE_XPOS+85, GREETING_TITLE_YPOS+80, (uint8_t *)"KLIKNIJ ABY URUCHOMIC:", LEFT_MODE);
	}
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(APPLY_BUTTON_XPOS, APPLY_BUTTON_YPOS, APPLY_BUTTON_WIDTH, APPLY_BUTTON_HEIGHT);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(APPLY_BUTTON_XPOS+20, APPLY_BUTTON_YPOS+(APPLY_BUTTON_HEIGHT/3), (uint8_t *)"START", LEFT_MODE);
}

void StartWindow_Touch_Detection(void)
{
	if((y > APPLY_BUTTON_YPOS) && (y < APPLY_BUTTON_YPOS + APPLY_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku Reverb */	
		if((x > APPLY_BUTTON_XPOS) && (x < APPLY_BUTTON_XPOS + APPLY_BUTTON_WIDTH))
		{	
				BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_OFF);
				Display_MainWindow();
		}
	}
}


void LCD_Multieffect_Init()
{
	BSP_TS_Init(480, 270);  /* Inicjalizacja panelu dotykowego */
	BSP_LCD_Init();						/* Inicjalizacja wyswietlacza LCD */
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FRAME_BUFFER);  /* Inicjalizacja warstwy LCD - bufor LCD w pamieci */
	BSP_LCD_DisplayOn();	/* Uruchomienie wyswietlacza */
	BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);    

	current_window = START_WINDOW;

	HAL_TIM_Base_Start_IT(&htim10);
	Display_StartWindow(0);
//	Display_MainWindow();
}

void Display_MainWindow(void)
{ 
	current_window = MAIN_WINDOW;
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	int i;
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	for(i=1; i<9; i++)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(BUTTON_XPOS(i), BUTTON_YPOS(i), BUTTON_WIDTH, BUTTON_HEIGHT);
		
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		if(i == 8) 
		{
			BSP_LCD_DisplayStringAt(BUTTON_XPOS(i)+2, BUTTON_YPOS(i)+(BUTTON_HEIGHT/2)-3, (uint8_t *)button_names[i-1], LEFT_MODE);
		}else
		{
			BSP_LCD_DisplayStringAt(BUTTON_XPOS(i)+15, BUTTON_YPOS(i)+(BUTTON_HEIGHT/2)-3, (uint8_t *)button_names[i-1], LEFT_MODE);
		}	
	}
}
	
void Multieffect(void)
{
	/* Inicjalizacja kodeka, SAI */
	if (BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_INPUT_LINE_1 , OUTPUT_DEVICE_HEADPHONE, I2S_AUDIOFREQ_44K  , DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) == AUDIO_OK)
	{
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	}
	else
	{
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"BLAD URZADZENIA AUDIO", CENTER_MODE);
		BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)" Sprobuj zresetowac urzadzenie", CENTER_MODE);
	}

	/* Inicjalizacja buforów SDRAM */
	is_codec_initialized = AUDIO_OK;
	memset((uint16_t*)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE*2);
	memset((uint16_t*)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE*4);

	memset(data_in, 0, AUDIO_BUFFER_SIZE);
	memset(data_out, 0, AUDIO_BUFFER_SIZE);
	
	audio_rec_buffer_state = BUFFER_OFFSET_NONE;

	/* Rozpoczecie nagrywania */
	BSP_AUDIO_IN_Record((uint16_t*)AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE);

	/* Odtworzenie */
	BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
	BSP_AUDIO_OUT_Play((uint16_t*)AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE*4);
	
	BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);

	//TODO Zmniejszyc rozmiar bufora AUDIO_BLOCK_SIZE
	
	 while (1)
	{
		/* Czekaj na nagranie polowy bloku */

		while(audio_rec_buffer_state != BUFFER_OFFSET_HALF)
		{
			HAL_Delay(1);
		}
		audio_rec_buffer_state = BUFFER_OFFSET_NONE;
		
		/* Skopiuj nagrany blok */
		memcpy((uint16_t *)(data_in + (count*AUDIO_BLOCK_SIZE/2)), (uint16_t *)(AUDIO_BUFFER_IN), AUDIO_BLOCK_SIZE);
		memcpy((uint16_t *)(data_out + (count*AUDIO_BLOCK_SIZE/2)), (uint16_t *)(data_in + (count*AUDIO_BLOCK_SIZE/2)), AUDIO_BLOCK_SIZE);
		

		/* Przetwórz próbki sygnalu wlaczonymi efektami */
		Signal_Processing(data_in, data_out, count);
		
		m = 0;
		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++) 
		{
			data_out_processed[m] = data_out[n];
			data_out_processed[m+1] = data_out[n];
			m += 2;
			sample1 = (int16_t)data_out[n];
		}
		
		/* Odtworz sygnal wyjsciowy */
		memcpy((uint16_t *)(AUDIO_BUFFER_OUT),
									(uint16_t *)(data_out_processed),
									AUDIO_BLOCK_SIZE*2);
		
		count += 1;
		
		
		/* Czekaj do konca nagrania bloku */
		while(audio_rec_buffer_state != BUFFER_OFFSET_FULL)
		{
			HAL_Delay(1);
		}
		audio_rec_buffer_state = BUFFER_OFFSET_NONE;
		
		/* Skopiuj 2gi nagrany blok */
		memcpy((uint16_t *)(data_in+(count*AUDIO_BLOCK_SIZE/2)), (uint16_t *)(AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE)), AUDIO_BLOCK_SIZE);
		memcpy((uint16_t *)(data_out + (count*AUDIO_BLOCK_SIZE/2)), (uint16_t *)(data_in+(count*AUDIO_BLOCK_SIZE/2)), AUDIO_BLOCK_SIZE);
		
		/* Przetwórz próbki sygnalu wlaczonymi efektami */
		Signal_Processing(data_in, data_out, count);
		
		m = 0;
		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++) 
		{	
			data_out_processed[m] = data_out[n];
			data_out_processed[m+1] = data_out[n];
			m += 2;
		}
		
		/* Odtworz sygnal wyjsciowy */
		memcpy((uint16_t *)(AUDIO_BUFFER_OUT + (AUDIO_BLOCK_SIZE*2)),
									(uint16_t *)(data_out_processed),
									AUDIO_BLOCK_SIZE*2);
			
		if(count == 43)
		{
			count = 0;
		}
		else
		{
				count += 1;
		}
	}
}

void Current_Window_Select(void)
{
		switch(current_window)
		{
			case MAIN_WINDOW:
				MainWindow_Touch_Detection();
				break;
			case VOLUME_WINDOW:
				VolumeWindow_Touch_Detection(x, y);
				break;
			case REVERB_WINDOW:
				ReverbWindow_Touch_Detection(x, y);
				break;
			case OVERDRIVE_WINDOW:
				OverdriveWindow_Touch_Detection(x, y);
				break;
			case DELAY_WINDOW:
				DelayWindow_Touch_Detection(x, y);
				break;			
			case FLANGER_WINDOW:
				FlangerWindow_Touch_Detection(x, y);
				break;		
			case CHORUS_WINDOW:
				ChorusWindow_Touch_Detection(x, y);
				break;		
			case TREMOLO_WINDOW:
				TremoloWindow_Touch_Detection(x, y);
				break;		
			case PITCHSHIFTER_WINDOW:
				PitchShifterWindow_Touch_Detection(x, y);
				break;		
			case START_WINDOW:
				StartWindow_Touch_Detection();
				break;		
		}
	
}


void MainWindow_Touch_Detection(void)
{
		/* Sprawdzenie 1ego rzedu przycisków */
		if((y > BUTTON_YPOS(1)) && (y < BUTTON_YPOS(1) + BUTTON_HEIGHT))
		{
				/* Sprawdzenie przycisku Reverb */	
				if((x > BUTTON_XPOS(1)) && (x < BUTTON_XPOS(1) + BUTTON_WIDTH))
				{
					  current_window = REVERB_WINDOW;
						Display_Reverb_Window();
				/* Sprawdzenie przycisku Overdrive */						
				}else if((x > BUTTON_XPOS(2)) && (x < BUTTON_XPOS(2) + BUTTON_WIDTH))
				{
						current_window = OVERDRIVE_WINDOW;
				  	Display_Overdrive_Window();
				/* Sprawdzenie przycisku Delay */						
				}else if((x > BUTTON_XPOS(3)) && (x < BUTTON_XPOS(3) + BUTTON_WIDTH))
				{
						current_window = DELAY_WINDOW;
						Display_Delay_Window();
				/* Sprawdzenie przycisku Glosnosc */					
				}else if((x > BUTTON_XPOS(4)) && (x < BUTTON_XPOS(4) + BUTTON_WIDTH))
				{
					 current_window = VOLUME_WINDOW;
					 Display_Volume_Window();
				}
		/* Sprawdzenie 2ego rzedu przycisków */			
		}else if((y > BUTTON_YPOS(5)) && (y < BUTTON_YPOS(5) + BUTTON_HEIGHT))
		{
				/* Sprawdzenie przycisku Flanger */				
				if((x > BUTTON_XPOS(5)) && (x < BUTTON_XPOS(5) + BUTTON_WIDTH))
				{
					 current_window = FLANGER_WINDOW;
					 Display_Flanger_Window();
				/* Sprawdzenie przycisku Chorus */					
				}else if((x > BUTTON_XPOS(6)) && (x < BUTTON_XPOS(6) + BUTTON_WIDTH))
				{
					  current_window = CHORUS_WINDOW;
						Display_Chorus_Window();
				/* Sprawdzenie przycisku Tremolo */					
				}else if((x > BUTTON_XPOS(7)) && (x < BUTTON_XPOS(7) + BUTTON_WIDTH))
				{
					 	current_window = TREMOLO_WINDOW;
						Display_Tremolo_Window();
				/* Sprawdzenie przycisku Pitch Shifter */			
				}else if((x > BUTTON_XPOS(8)) && (x < BUTTON_XPOS(8) + BUTTON_WIDTH))
				{		
					 	current_window = PITCHSHIFTER_WINDOW;
						Display_PitchShifter_Window();
				}
		}
}


void Signal_Processing(uint16_t *data_in, uint16_t *data_out, uint32_t count)
{
	//PitchShifter(data_in, data_out, count);
	//Overdrive(data_out, count);
	//Chorus(data_in, data_out, count);
	//Flanger(data_in, data_out, count);
	//Tremolo(data_out, count);
	//Delay(data_in, data_out,  count);
	
	Reverb(data_in, data_out, count);
}


/**
  * @brief Manages the DMA Transfer complete interrupt.
  * @param None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_FULL;
  return;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_HALF;
  return;
}

/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_CallBack(void)
{
  /* This function is called when an Interrupt due to transfer error on or peripheral
     error occurs. */
  /* Display message on the LCD screen */
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"       DMA  ERROR     ", CENTER_MODE);

  /* Stop the program with an infinite loop */
  while (BSP_PB_GetState(BUTTON_KEY) != RESET)
  {
    return;
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{		
		if(htim->Instance == TIM10)
		{
				/* Sprawdz jaki jest stan wyswietlacza dotykowego oraz czy uzytkownik go dotknal */
				BSP_TS_GetState(&ts);
				if(ts.touchDetected)
				{
					if(is_codec_initialized == AUDIO_OK)
						BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
					/* Pobierz koordynaty dotknietego ekranu */
					x = ts.touchX[0];
					y = ts.touchY[0];
					/* Sprawdz, które okno jest obecnie aktywne */
					Current_Window_Select();
					is_button_active = BUTTON_ACTIVE;
					if(is_codec_initialized == AUDIO_OK)
							BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_OFF);
				}
				else
				{
					is_button_active = BUTTON_NOT_ACTIVE;
				}
			
			
				if(current_window == START_WINDOW)
				{
					if(second_counter == 10)
					{
						Display_StartWindow(1);
					}else if(second_counter == 20)
					{
						second_counter = 0;
						Display_StartWindow(0);
					}								
					second_counter += 1;
				}  
		}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

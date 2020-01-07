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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/**
  * Stany zapelnienia bufora przetwarzania audio
  */
typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;

/**
  * Stan (nazwa) obecnie wyswietlanego okna aplikacji
  */
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
void Set_Default_Parameters(void);
void Load_Effects_Parameters(void);
void Save_Effects_Parameters(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  *  Zmienne obslugi i inicjalizacji wyswietlacza LCD oraz panelu dotykowego
  */
TS_StateTypeDef 			ts;
char xTouchStr[10];
uint16_t x, y;
uint8_t current_window;
uint8_t is_button_active= BUTTON_NOT_ACTIVE;

/**
  * Zmienne przetwarzania sygnalu audio
  */
uint8_t  audio_rec_buffer_state;
uint8_t is_codec_initialized = AUDIO_ERROR;
uint16_t data_in[AUDIO_BUFFER_SIZE];
uint16_t data_out[AUDIO_BUFFER_SIZE];
uint16_t data_out_processed[2*AUDIO_BLOCK_SIZE];

/**
  * Zmienne obslugi licznika TIM10
  */
uint32_t count = 0;
uint8_t second_counter = 0;

/**
  * Zmienne obslugi karty SD
  */
uint8_t SD_state = MSD_OK;
uint32_t sd_data_buffer[BUFFER_WORDS_SIZE];

int16_t sample1;
int16_t sample2;
int16_t sample1x;
int16_t sample2x;


/**
  * Tablica nazw przciskow menu glownego aplikacji
  */
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

/* USER CODE END 0 */

/**
  * @brief  Punkt wejsciowy aplikacji.
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

	/* Initializes the SDRAM device */
	BSP_SDRAM_Init(); 
	/* Enable the CRC Module */
	__HAL_RCC_CRC_CLK_ENABLE(); 

	/**
		* Inicjalizacja sterownika obslugi karty SD
		*/
	SD_state = BSP_SD_Init();
	/**
  * Inicjalizacja wyswietlacza LCD oraz panelu dotykowego
  */
	LCD_Multieffect_Init();
	/**
  * Inicjalizacja algorytmu wokodera fazowego
  */
//	PhaseVocoderInit();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		

    /* USER CODE BEGIN 3 */
		/**
		* Uruchomienie przetwarzania sygnalu w czasie rzeczywistym
		*/
		Multieffect();
  }
  /* USER CODE END 3 */
}

/**
  * @brief Konfiguracja zegara systemowego
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
  * @brief Funkcja inicjalizujaca CRC
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
  * @brief Funkcja inicjalizujaca DMA2D
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
  * @brief Funkcja inicjalizujaca licznik TIM10.
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
  * @brief Funkcja inicjalizaujaca GPIO.
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

/**
  * @brief  Wyswietla okno startowe aplikacji
  * @param  is_blinking: Okresla kolor napisu startowego
  *          Przyjmuje on jedna z dwoch wartosci:
  *            @arg  0: kolor czarny napisu
  *            @arg  1: kolor bialy napisu
  * @retval Brak
  */
void Display_StartWindow(uint8_t is_blinking) 
{
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(GREETING_TITLE_XPOS, GREETING_TITLE_YPOS, (uint8_t *)"MULTIEFEKT GITAROWY", LEFT_MODE);
	BSP_LCD_SetFont(&Font12);
	
	/* Zmiana koloru napisu 'KLIKNIJ ABY URUCHOMIC' */
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
	
	/* Sprawdzenie stanu kary SD oraz ewentualne zaladowanie danych z jej pamieci */
	if (SD_state != MSD_OK)
	{
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		if(SD_state == MSD_ERROR_SD_NOT_PRESENT)
		{
			BSP_LCD_DisplayStringAt(195, 160, (uint8_t *)"Brak karty SD", LEFT_MODE);
		}else
		{
			BSP_LCD_DisplayStringAt(195, 160, (uint8_t *)"Blad karty SD.", LEFT_MODE);
		}
	}else
	{
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_DisplayStringAt(195, 160, (uint8_t *)"Karta SD obecna", LEFT_MODE);
		
		Load_Effects_Parameters();
	}
}

/**
  * @brief  Implementuje funkcjonalnosc wcisniecia przyciskow okna startowego
  * @param Brak
  * @retval Brak
  */
void StartWindow_Touch_Detection(void)
{
	if((y > APPLY_BUTTON_YPOS) && (y < APPLY_BUTTON_YPOS + APPLY_BUTTON_HEIGHT))
	{
		/* Sprawdzenie przycisku START */	
		if((x > APPLY_BUTTON_XPOS) && (x < APPLY_BUTTON_XPOS + APPLY_BUTTON_WIDTH))
		{	
			BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_OFF);
			Display_MainWindow();
		}
	}
}

/**
  * @brief  Inicjalizacja wyswietlacza LCD oraz panelu dotykowego
  * @param  Brak
  * @retval Brak
  */
void LCD_Multieffect_Init(void)
{
	/* Inicjalizacja panelu dotykowego */
	BSP_TS_Init(480, 270);  
	/* Inicjalizacja wyswietlacza LCD */
	BSP_LCD_Init();						
	/* Inicjalizacja warstwy LCD - bufor LCD w pamieci */
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FRAME_BUFFER);
	/* Uruchomienie wyswietlacza */
	BSP_LCD_DisplayOn();
	BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);    

	current_window = START_WINDOW;

	HAL_TIM_Base_Start_IT(&htim10);
	Display_StartWindow(0);
}

/**
  * @brief  Wyswietla glowne okno menu aplikacji
  * @param  Brak
  * @retval Brak
  */
void Display_MainWindow(void)
{ 
	current_window = MAIN_WINDOW;
	
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
	int i;
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	
	/* Wyswietlenie przyciskow menu glownego */
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
	
	/* Wyswietlenie przycisku przywrocenia domyslnych wartosci parametrow efektow */
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(SET_DEFAULT_BUTTON_XPOS-5, SET_DEFAULT_BUTTON_YPOS-5, SET_DEFAULT_BUTTON_WIDTH+10, SET_DEFAULT_BUTTON_HEIGHT+10);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(SET_DEFAULT_BUTTON_XPOS, SET_DEFAULT_BUTTON_YPOS, SET_DEFAULT_BUTTON_WIDTH, SET_DEFAULT_BUTTON_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

		BSP_LCD_SetFont(&Font8);
		BSP_LCD_DisplayStringAt(SET_DEFAULT_BUTTON_XPOS+5, SET_DEFAULT_BUTTON_YPOS+(SET_DEFAULT_BUTTON_HEIGHT/2)-3, (uint8_t *)"Przywroc domyslne", LEFT_MODE);
	
	/* Wyswietlenie przycisku zapisu danych na karte SD w zaleznosci od jej stanu */
	if (SD_state == MSD_OK)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(SD_CARD_SAVE_XPOS-5, SD_CARD_SAVE_YPOS-5, SD_CARD_SAVE_WIDTH+10, SD_CARD_SAVE_HEIGHT+10);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(SD_CARD_SAVE_XPOS, SD_CARD_SAVE_YPOS, SD_CARD_SAVE_WIDTH, SD_CARD_SAVE_HEIGHT);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

		BSP_LCD_SetFont(&Font8);
		BSP_LCD_DisplayStringAt(SD_CARD_SAVE_XPOS+5, SD_CARD_SAVE_YPOS+(SD_CARD_SAVE_HEIGHT/2)-3, (uint8_t *)"Zapisz na karcie SD", LEFT_MODE);
	}
	
	/* Wyswietlenie napisu tytulowego */
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_DisplayStringAt(85, 8, (uint8_t *)"MULTIEFEKT GITAROWY", LEFT_MODE);
}

/**
  * @brief  Implementacja funkcjonalnosci wcisniecia przyciskow okna glownego
  * @param  Brak
  * @retval Brak
  */
void MainWindow_Touch_Detection(void)
{
	/* Sprawdzenie 1ego rzedu przycisk�w */
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
	/* Sprawdzenie 2ego rzedu przycisk�w */			
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
		
	/* Sprawdzenie przycisku przywrocenia domyslnych wartosci parametrow efektow */	
	}else if((y > SET_DEFAULT_BUTTON_YPOS) && (y < SET_DEFAULT_BUTTON_YPOS + SD_CARD_SAVE_HEIGHT))
	{
		if((x > SET_DEFAULT_BUTTON_XPOS) && (x < SET_DEFAULT_BUTTON_XPOS + SET_DEFAULT_BUTTON_WIDTH))
		{
			Set_Default_Parameters();
		}
	/* Sprawdzenie przycisku zapisu parametr�w efekt�w na karcie SD */	
	}else if((y > SD_CARD_SAVE_YPOS) && (y < SD_CARD_SAVE_YPOS + SD_CARD_SAVE_HEIGHT))
	{
		if((x > SD_CARD_SAVE_XPOS) && (x < SD_CARD_SAVE_XPOS + SD_CARD_SAVE_WIDTH))
		{
			if (SD_state != MSD_OK)
			{
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
				BSP_LCD_SetFont(&Font12);
				BSP_LCD_DisplayStringAt(SD_INFO_XPOS, SD_INFO_YPOS, (uint8_t *)"BLAD SD", LEFT_MODE);
			}
			else
			{
				Save_Effects_Parameters();
			}
		}
	}
}

/**
  * @brief  Wybor funkcji obslugi przyciskow panelu dotykowego w zaleznosci od obecnie wyswietlanego okna
  * @param  Brak
  * @retval Brak
  */
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

/**
  * @brief  Realizacja przetwarzania sygnalu audio
  * @param Brak
  * @retval Brak
  */
void Multieffect(void)
{
	uint16_t m, n;
	
	/* Inicjalizacja kodeka oraz SAI */
	if (BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_INPUT_LINE_1, OUTPUT_DEVICE_HEADPHONE, I2S_AUDIOFREQ_44K , DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) == AUDIO_OK)
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

	/* Inicjalizacja bufor�w SDRAM */
	is_codec_initialized = AUDIO_OK;
	memset((uint16_t*)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE*2);
	memset((uint16_t*)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE*4);

	memset(data_in, 0, AUDIO_BUFFER_SIZE);
	memset(data_out, 0, AUDIO_BUFFER_SIZE);
	
	audio_rec_buffer_state = BUFFER_OFFSET_NONE;

	/* Rozpoczecie nagrywania */
	BSP_AUDIO_IN_Record((uint16_t*)AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE);

	/* Rownolegle odtwarzanie probek */
	BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
	BSP_AUDIO_OUT_Play((uint16_t*)AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE*4);
	
	BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
	
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
		
		/* Przetw�rz pr�bki sygnalu wlaczonymi efektami */
		Signal_Processing(data_in, data_out, count);

		
		/* Powielenie probek w buforze, aby dzwiek byl slyszalny na obu kanalach */
		m = 0;
		for(n = count*AUDIO_BLOCK_SIZE/2; n < (count+1)*AUDIO_BLOCK_SIZE/2; n++) 
		{
			data_out_processed[m] = data_out[n];
			data_out_processed[m+1] = data_out[n];
			m += 2;
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
		
		/* Przetw�rz pr�bki sygnalu wlaczonymi efektami */
		Signal_Processing(data_in, data_out, count);
		
		/* Powielenie probek w buforze, aby dzwiek byl slyszalny na obu kanalach */
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
			
		/* Aktualizacja licznika obecnie przetwarzanego bloku probek */
		if(count == 351)
		{
			count = 0;
		}
		else
		{
			count += 1;
		}
	}
}

/**
  * @brief  Realizacja przetwarzania sygnalu wejsciowego w petli efektow
  * @param  *data_in: wskaznik na bufor oryginalego sygnalu wejsciowego
  * @param  *data_out: wskaznik na bufor przetworzonego sygnalu wejsciowego
  * @param  count: obecna ramka sygnalu audio
  * @retval Brak
  */
void Signal_Processing(uint16_t *data_in, uint16_t *data_out, uint32_t count)
{
	PitchShifter(data_out, count);
	Overdrive(data_out, count);
	Chorus(data_in, data_out, count);
	Flanger(data_in, data_out, count);
	Tremolo(data_out, count);
	Delay(data_in, data_out,  count);
	Reverb(data_in, data_out, count);
}

/**
  * @brief Zarzadza przerwaniem DMA w przypadku zapelnienia calego bufora probek audio.
  * @param None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_FULL;
  return;
}

/**
  * @brief  Zarzadza przerwaniem DMA w przypadku zapelnienia polowy bufora probek audio.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_HALF;
  return;
}

/**
  * @brief  Funkcja callback wywolywana w przypadku bledu wejsciowego audio.
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


/**
	* @brief  Wczytanie i ustawienie domyslnych wartosci parametrow efektow do aplikacji
  * @param  Brak 
  * @retval Brak
  */
void Set_Default_Parameters(void)
{
	Set_Parameters_Volume(70);
	Set_Parameters_Overdrive(0.5, 0.5, NOT_ACTIVE);
	Set_Parameters_Delay(500, 0.5, SOI, NOT_ACTIVE);
	Set_Parameters_Flanger(10, 0.5, 0.5, SOI, NOT_ACTIVE);
	Set_Parameters_Chorus(5, 0.2, 0.2, 0.5, 0.5, SOI, NOT_ACTIVE);
	Set_Parameters_Tremolo(5, 0.5, SIN, NOT_ACTIVE);
	Set_Parameters_Reverb(50, 0.5, 10, 10, NOT_ACTIVE);
	
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(SD_INFO_XPOS, SD_INFO_YPOS, (uint8_t *)"OK", LEFT_MODE);
}

/**
	* @brief  Zaladowanie parametrow efektow z karty SD do aplikacji
  * @param  Brak 
  * @retval Brak
  */
void Load_Effects_Parameters(void)
{
	/* Czekaj az karta SD bedzie gotowa do wykonania kolejnej operacji */
	while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
	{
	}
	
	if (SD_state != MSD_OK)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_DisplayStringAt(195, 160, (uint8_t *)"Blad ladowania danych z karty SD.", LEFT_MODE);
	}else
	{
		/* Odczyt danych z kart SD i zapis do bufora */
		SD_state = BSP_SD_ReadBlocks(sd_data_buffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 10000);
				
		/* Czekaj az karta SD bedzie gotowa do wykonania kolejnej operacji */
		while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
		{
		}
		
		/* Informacja, czy operacja sie udala */
		if (SD_state != MSD_OK)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_SetFont(&Font12);
			BSP_LCD_DisplayStringAt(195, 160, (uint8_t *)"Blad ladowania danych z karty SD.", LEFT_MODE);
		}else
		{
			/* Ustawienie wartosci dla parametrow kazdego z efektow oraz glosnosci jesli istnieja takowe na karcie SD*/
			if(sd_data_buffer[0] == SD_CODE)
			{
				Set_Parameters_Volume(sd_data_buffer[1]);
				Set_Parameters_Overdrive((float)(sd_data_buffer[2])/100, (float)(sd_data_buffer[3])/100, sd_data_buffer[4]);
				Set_Parameters_Delay(sd_data_buffer[5], (float)(sd_data_buffer[6])/100, sd_data_buffer[7], sd_data_buffer[8]);
				Set_Parameters_Flanger(sd_data_buffer[9], (float)(sd_data_buffer[10])/100, (float)(sd_data_buffer[11])/100, sd_data_buffer[12], sd_data_buffer[13]);
				Set_Parameters_Chorus(sd_data_buffer[14], (float)(sd_data_buffer[15])/100, (float)(sd_data_buffer[16])/100, (float)(sd_data_buffer[17])/100, (float)(sd_data_buffer[18])/100, sd_data_buffer[19], sd_data_buffer[20]);
				Set_Parameters_Tremolo(sd_data_buffer[21], (float)(sd_data_buffer[22])/100, sd_data_buffer[23], sd_data_buffer[24]);
				Set_Parameters_Reverb(sd_data_buffer[25], (float)(sd_data_buffer[26])/100, sd_data_buffer[27], sd_data_buffer[28], sd_data_buffer[29]);
				Set_Parametres_PitchShifter((int8_t)(sd_data_buffer[30]), (float)(sd_data_buffer[31])/100, sd_data_buffer[32], sd_data_buffer[33]);
				
				BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
				BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
				BSP_LCD_SetFont(&Font12);
				BSP_LCD_DisplayStringAt(155, 160, (uint8_t *)"Zaladowano dane z karty SD", LEFT_MODE);
				
				memset(sd_data_buffer, 0, BUFFER_WORDS_SIZE);
			}else
			{
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
				BSP_LCD_SetFont(&Font12);
				BSP_LCD_DisplayStringAt(195, 160, (uint8_t *)"Brak danych do zaladowania z karty SD", LEFT_MODE);
			}
		}
	}
}

/**
	* @brief  Zaladowanie obecnych parametrow efektow z aplikacji na karte SD
  * @param  Brak 
  * @retval Brak
  */
void Save_Effects_Parameters(void)
{
	/* Czekaj az karta SD bedzie gotowa do wykonania kolejnej operacji */
	while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
	{
	}
	
	if (SD_state != MSD_OK)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_DisplayStringAt(SD_INFO_XPOS, SD_INFO_YPOS, (uint8_t *)"BLAD SD", LEFT_MODE);
	}else
	{	
	int sd_buff_n = 0;
	/* Wypelnienie bufora danymi wartosci parametrow dla kazdego z efektow */
	for(sd_buff_n = 0; sd_buff_n < 34; sd_buff_n++)
	{
		if(sd_buff_n == 0)
		{
			sd_data_buffer[sd_buff_n] = SD_CODE;	
		}
		else if(sd_buff_n == 1)
		{
			sd_data_buffer[sd_buff_n] = Get_Parameter_Volume();	
		}
		else if(sd_buff_n > 1 && sd_buff_n <= 4)
		{
			sd_data_buffer[sd_buff_n] = Get_Parameter_Overdrive(sd_buff_n - 2);	
		}
		else if(sd_buff_n > 4 && sd_buff_n <= 8)
		{
			sd_data_buffer[sd_buff_n] = Get_Parameter_Delay(sd_buff_n - 5);	
		}
		else if(sd_buff_n > 8 && sd_buff_n <= 13)
		{
			sd_data_buffer[sd_buff_n] = Get_Parameter_Flanger(sd_buff_n - 9);	
		}
		else if(sd_buff_n > 13 && sd_buff_n <= 20)
		{
			sd_data_buffer[sd_buff_n] = Get_Parameter_Chorus(sd_buff_n - 14);	
		}
		else if(sd_buff_n > 20 && sd_buff_n <= 24)
		{
			sd_data_buffer[sd_buff_n] = Get_Parameter_Tremolo(sd_buff_n - 21);	
		}
		else if(sd_buff_n > 24 && sd_buff_n <= 29)
		{
			sd_data_buffer[sd_buff_n] = Get_Parameter_Reverb(sd_buff_n - 25);	
		}
		else if(sd_buff_n > 29 && sd_buff_n <= 33)
		{
			sd_data_buffer[sd_buff_n] = Get_Parameter_PitchShifter(sd_buff_n - 30);	
		}
	}
	
		/* Zapis danych z bufora na karte SD  */
		SD_state = BSP_SD_WriteBlocks(sd_data_buffer, BLOCK_START_ADDR, NUM_OF_BLOCKS, 10000);
	
		/* Czekaj az karta SD bedzie gotowa do wykonania kolejnej operacji */
		while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
		{
		}
		
		/* Informacja, czy operacja sie udala */
		if (SD_state != MSD_OK)
		{
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_SetFont(&Font12);
			BSP_LCD_DisplayStringAt(SD_INFO_XPOS, SD_INFO_YPOS, (uint8_t *)"BLAD SD", LEFT_MODE);
		}else
		{ 
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
			BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_SetFont(&Font12);
			BSP_LCD_DisplayStringAt(SD_INFO_XPOS, SD_INFO_YPOS, (uint8_t *)"OK", LEFT_MODE);
			
			memset(sd_data_buffer, 0, BUFFER_WORDS_SIZE);
		}
	}
}

/**
* @brief  Realizacja przerwania od licznika TIM10: sprawdzenie stanu panelu dotykowego i obsluga interakcji uzytkownika
  * @param  *htim: wskaznik na licznik, ktory zglasza przerwanie 
  * @retval Brak
  */
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
			/* Sprawdz, kt�re okno jest obecnie aktywne */
			Current_Window_Select();
			/* Ustawienie flagi 'Przycisk wcisniety' */
			is_button_active = BUTTON_ACTIVE;
			if(is_codec_initialized == AUDIO_OK)
				BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_OFF);
		}
		else
		{
			/* Ustawienie flagi 'Przycisk nie wcisniety' */
			is_button_active = BUTTON_NOT_ACTIVE;
		}
	
		/* Realizacja animacja 'migania' napisu startowego */
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

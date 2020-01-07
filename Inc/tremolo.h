/* USER CODE BEGIN Header */
/**
  *************************************************************************************
  * @file           : tremolo.h
  * @brief         : Naglowek pliku tremolo.c.
  *  	Zawiera makro, pliki naglowkowe oraz prototypy funkcji pliku tremolo.c
  *************************************************************************************
  */
/* USER CODE END Header */

#ifndef __TREMOLO_H
#define __TREMOLO_H
#endif

#include "GUI.h"
#include "arm_math.h"
#include "arm_common_tables.h"

typedef enum
{
	SIN = 0,
	TRI = 1,
	SQU = 2,
	
}TREMOLO_SHAPE_StateTypeDef;


void Display_Tremolo_Window(void);
void TremoloWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parameters_Tremolo(void);
void Display_On_Off_Info_Tremolo(void);
void Tremolo(uint16_t *data_out, uint32_t count);
float Square_Signal(uint16_t x, uint16_t period);
float Triangle_Signal(uint16_t x, uint16_t period);
uint32_t Get_Parameter_Tremolo(uint8_t parameter);
void Set_Parameters_Tremolo(uint8_t rate, float depth, uint8_t shape, uint8_t is_active);

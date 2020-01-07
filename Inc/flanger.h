/* USER CODE BEGIN Header */
/**
  *************************************************************************************
  * @file           : flanger.h
  * @brief         : Naglowek pliku flanger.c.
  *  	Zawiera makro, pliki naglowkowe oraz prototypy funkcji pliku flanger.c
  *************************************************************************************
  */
/* USER CODE END Header */

#ifndef __FLANGER_H
#define __FLANGER_H
#endif

#include "GUI.h"
#include "arm_math.h"
#include "arm_common_tables.h"


void Display_Flanger_Window(void);
void FlangerWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parameters_Flanger(void);
void Display_On_Off_Info_Flanger(void);
void Flanger(uint16_t *data_in, uint16_t* data_out, uint32_t count);
uint32_t Get_Parameter_Flanger(uint8_t parameter);
void Set_Parameters_Flanger(uint8_t delay, float rate, float depth, uint8_t version, uint8_t is_active);

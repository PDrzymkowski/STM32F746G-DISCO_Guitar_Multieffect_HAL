/* USER CODE BEGIN Header */
/**
  *************************************************************************************
  * @file           : delay.h
  * @brief         : Naglowek pliku delay.c.
  *  	Zawiera makro, pliki naglowkowe oraz prototypy funkcji pliku delay.c
  *************************************************************************************
  */
/* USER CODE END Header */

#ifndef __DELAY_H
#define __DELAY_H
#endif

#include "GUI.h"


void Display_Delay_Window(void);
void DelayWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parameters_Delay(void);
void Display_On_Off_Info_Delay(void);
void Delay(uint16_t *data_in, uint16_t *data_out, uint32_t count);
uint32_t Get_Parameter_Delay(uint8_t parameter);
void Set_Parameters_Delay(uint16_t delay, float gain, uint8_t version, uint8_t is_active);

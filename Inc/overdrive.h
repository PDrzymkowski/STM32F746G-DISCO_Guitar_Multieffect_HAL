/* USER CODE BEGIN Header */
/**
  *************************************************************************************
  * @file           : overdrive.h
  * @brief         : Naglowek pliku overdrive.c.
  *  	Zawiera makro, pliki naglowkowe oraz prototypy funkcji pliku overdrive.c
  *************************************************************************************
  */
/* USER CODE END Header */

#ifndef __OVERDRIVE_H
#define __OVERDRIVE_H
#endif

#include "GUI.h"


void Display_Overdrive_Window(void);
void OverdriveWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parameters_Overdrive(void);
void Display_On_Off_Info_Overdrive(void);
void Overdrive(uint16_t *data_out, uint32_t count);
uint32_t Get_Parameter_Overdrive(uint8_t parameter);
void Set_Parameters_Overdrive(float clip_value, float gain, uint8_t is_active);

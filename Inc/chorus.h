/* USER CODE BEGIN Header */
/**
  *************************************************************************************
  * @file           : chorus.h
  * @brief         : Naglowek pliku chorus.c.
  *  	Zawiera makro, pliki naglowkowe oraz prototypy funkcji pliku chorus.c
  *************************************************************************************
  */
/* USER CODE END Header */

#ifndef __CHORUS_H
#define __CHORUS_H
#endif

#include "GUI.h"



void Display_Chorus_Window(void);
void ChorusWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parameters_Chorus(void);
void Display_On_Off_Info_Chorus(void);
void Chorus(uint16_t *data_in, uint16_t* data_out, uint32_t count);
void Set_Parameters_Chorus(uint8_t delay, float rate1, float rate2, float depth1, float depth2, uint8_t version, uint8_t is_active);
uint32_t Get_Parameter_Chorus(uint8_t parameter);

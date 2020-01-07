/* USER CODE BEGIN Header */
/**
  *************************************************************************************
  * @file           : volume.h
  * @brief         : Naglowek pliku volume.c.
  *  	Zawiera makro, pliki naglowkowe oraz prototypy funkcji pliku volume.c
  *************************************************************************************
  */
/* USER CODE END Header */

#ifndef __VOLUME_H
#define __VOLUME_H
#endif

#include "GUI.h"



void Display_Volume_Window(void);
void VolumeWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Volume(void);
uint32_t Get_Parameter_Volume(void);
void Set_Parameters_Volume(uint8_t volume);

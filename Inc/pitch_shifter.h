/* USER CODE BEGIN Header */
/**
  *************************************************************************************
  * @file           : pitch_shifter.h
  * @brief         : Naglowek pliku pitch_shifter.c.
  *  	Zawiera makro, pliki naglowkowe oraz prototypy funkcji pliku 
	*		pitch_shifter.c
  *************************************************************************************
  */
/* USER CODE END Header */

#ifndef __PITCH_SHIFTER_H
#define __PITCH_SHIFTER_H
#endif

#include "GUI.h"
//#include "phase_vocoder.h"



typedef enum
{
	ADD = 0,
	REPLACE = 1,
	
}PITCHSHIFTER_MODE_StateTypeDef;



void Display_PitchShifter_Window(void);
void PitchShifterWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parametres_PitchShifter(void);
void Display_On_Off_Info_PitchShifter(void);
void PitchShifter(uint16_t *data, int32_t count);
uint32_t Get_Parameter_PitchShifter(uint8_t parameter);
void Set_Parametres_PitchShifter(int8_t pitch, float gain, uint8_t mode, uint8_t is_active);

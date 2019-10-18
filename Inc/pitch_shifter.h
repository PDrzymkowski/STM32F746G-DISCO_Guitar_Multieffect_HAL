#ifndef __PITCH_SHIFTER_H
#define __PITCH_SHIFTER_H
#endif

#include "GUI.h"


typedef enum
{
	ADD = 0,
	REPLACE = 1,
	
}PITCHSHIFTER_MODE_StateTypeDef;





void Display_PitchShifter_Window(void);
void PitchShifterWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parametres_PitchShifter(void);
void Display_On_Off_Info_PitchShifter(void);
void PitchShifter(void);

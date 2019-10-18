#ifndef __TREMOLO_H
#define __TREMOLO_H
#endif

#include "GUI.h"

	
typedef enum
{
	SIN = 0,
	TRI = 1,
	SQU = 2,
	
}TREMOLO_SHAPE_StateTypeDef;



void Display_Tremolo_Window(void);
void TremoloWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parametres_Tremolo(void);
void Display_On_Off_Info_Tremolo(void);
void Tremolo(void);

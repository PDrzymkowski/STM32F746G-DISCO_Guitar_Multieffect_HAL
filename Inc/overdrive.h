#ifndef __OVERDRIVE_H
#define __OVERDRIVE_H
#endif

#include "GUI.h"



#define CLIP_VALUE_BUTTON_YPOS								  95
#define CLIP_VALUE_BUTTON_WIDTH								  35
#define CLIP_VALUE_BUTTON_HEIGHT							  30


void Display_Overdrive_Window(void);
void OverdriveWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parametres_Overdrive(void);
void Display_On_Off_Info_Overdrive(void);
void Overdrive(uint16_t *data_out, uint8_t count);

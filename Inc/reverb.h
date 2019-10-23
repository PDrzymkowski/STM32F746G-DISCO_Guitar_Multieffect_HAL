#ifndef __REVERB_H
#define __REVERB_H
#endif

#include "GUI.h"


void Display_Reverb_Window(void);
void ReverbWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parametres_Reverb(void);
void Display_On_Off_Info_Reverb(void);
void Reverb(uint16_t *data);

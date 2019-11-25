#ifndef __DELAY_H
#define __DELAY_H
#endif

#include "GUI.h"


void Display_Delay_Window(void);
void DelayWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parametres_Delay(void);
void Display_On_Off_Info_Delay(void);
void Delay(uint16_t *data_in, uint16_t *data_out, uint32_t count);

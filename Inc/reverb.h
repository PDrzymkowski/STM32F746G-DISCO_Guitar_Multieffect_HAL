#ifndef __REVERB_H
#define __REVERB_H
#endif

#include "GUI.h"

#define COMB_1_OUT_MAX_SIZE				4411
#define COMB_2_OUT_MAX_SIZE				5161
#define COMB_3_OUT_MAX_SIZE				5866
#define COMB_4_OUT_MAX_SIZE				6616
#define COMB_SECT_OUT_MAX_SIZE		884
#define ALL_PASS_OUT_MAX_SIZE				COMB_SECT_OUT_MAX_SIZE


void Display_Reverb_Window(void);
void ReverbWindow_Touch_Detection(uint16_t x, uint16_t y);
void Display_Current_Parametres_Reverb(void);
void Display_On_Off_Info_Reverb(void);
void Reverb(uint16_t *data_in, uint16_t* data_out, uint32_t count);

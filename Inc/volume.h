#ifndef __VOLUME_H
#define __VOLUME_H
#endif

#include "main.h"

#define VOLUME_UP_BUTTON_XPOS		300
#define VOLUME_UP_BUTTON_YPOS		100
#define VOLUME_DOWN_BUTTON_XPOS		150
#define VOLUME_DOWN_BUTTON_YPOS		100
#define VOLUME_BUTTON_WIDTH		50
#define VOLUME_BUTTON_HEIGHT		35
#define APPLY_BUTTON_XPOS				205
#define APPLY_BUTTON_YPOS			170
#define APPLY_BUTTON_WIDTH			100
#define APPLY_BUTTON_HEIGHT			35

void Display_Volume_Window(void);
void VolumeWindow_Touch_Detection(uint16_t x, uint16_t y);

#ifndef __GUI_H
#define __GUI_H
#endif

#include "main.h"


/* Polozenie przycisk�w */
#define DELAY_DOWN_BUTTON_XPOS							30
#define CLIP_VALUE_DOWN_BUTTON_XPOS		DELAY_DOWN_BUTTON_XPOS	
#define PITCH_DOWN_BUTTON_XPOS							DELAY_DOWN_BUTTON_XPOS	
#define ROOM_SIZE_DOWN_BUTTON_XPOS			DELAY_DOWN_BUTTON_XPOS	
#define PRE_DELAY1_DOWN_BUTTON_XPOS		DELAY_DOWN_BUTTON_XPOS	
#define RATE1_DOWN_BUTTON_XPOS							DELAY_DOWN_BUTTON_XPOS
#define DEPTH_CH_DOWN_BUTTON_XPOS				DELAY_DOWN_BUTTON_XPOS

#define DELAY_UP_BUTTON_XPOS										170
#define CLIP_VALUE_UP_BUTTON_XPOS					DELAY_UP_BUTTON_XPOS
#define PITCH_UP_BUTTON_XPOS										DELAY_UP_BUTTON_XPOS
#define ROOM_SIZE_UP_BUTTON_XPOS						DELAY_UP_BUTTON_XPOS		
#define PRE_DELAY1_UP_BUTTON_XPOS				DELAY_UP_BUTTON_XPOS	
#define RATE1_UP_BUTTON_XPOS										DELAY_UP_BUTTON_XPOS
#define DEPTH_CH_UP_BUTTON_XPOS						DELAY_UP_BUTTON_XPOS

#define GAIN_DOWN_BUTTON_XPOS								230
#define RATE2_DOWN_BUTTON_XPOS							GAIN_DOWN_BUTTON_XPOS
#define DEPTH_DOWN_BUTTON_XPOS							GAIN_DOWN_BUTTON_XPOS
#define PRE_DELAY2_DOWN_BUTTON_XPOS		GAIN_DOWN_BUTTON_XPOS	
#define DELAY_CH_DOWN_BUTTON_XPOS				GAIN_DOWN_BUTTON_XPOS

#define GAIN_UP_BUTTON_XPOS											370
#define RATE2_UP_BUTTON_XPOS										GAIN_UP_BUTTON_XPOS
#define DEPTH_UP_BUTTON_XPOS										GAIN_UP_BUTTON_XPOS	
#define	PRE_DELAY2_UP_BUTTON_XPOS				GAIN_UP_BUTTON_XPOS	
#define DELAY_CH_UP_BUTTON_XPOS						GAIN_UP_BUTTON_XPOS

#define VERSION_BUTTON_XPOS											DELAY_DOWN_BUTTON_XPOS	+ 60
#define MODE_BUTTON_XPOS													PITCH_DOWN_BUTTON_XPOS	+ 30
#define SHAPE_BUTTON_XPOS												RATE1_DOWN_BUTTON_XPOS	+ 60

#define PARAM1_BUTTON_YPOS											110
#define PARAM2_BUTTON_YPOS											200

#define PARAM1_CH_BUTTON_YPOS									95
#define PARAM2_CH_BUTTON_YPOS								160
#define PARAM3_CH_BUTTON_YPOS								220

#define DELAY_10_BUTTON_YPOS										PARAM1_BUTTON_YPOS + 35
#define DELAY_10_DOWN_BUTTON_XPOS				DELAY_DOWN_BUTTON_XPOS + 5
#define DELAY_10_UP_BUTTON_XPOS							DELAY_UP_BUTTON_XPOS + 5

#define BACK_BUTTON_XPOS															10
#define BACK_BUTTON_YPOS															5

#define BUTTON_XPOS(i)    													  	  (((i%4) * BSP_LCD_GetXSize()) / 4) + 10
#define BUTTON_YPOS(i)      													  ((i < 5)?  ((BSP_LCD_GetYSize() - BUTTON_HEIGHT - 160)): ((BSP_LCD_GetYSize() - BUTTON_HEIGHT - 60)))

#define ON_OFF_BUTTON_XPOS												420
#define ON_OFF_BUTTON_YPOS											BACK_BUTTON_YPOS + 10

#define VOLUME_DOWN_BUTTON_XPOS					150
#define VOLUME_UP_BUTTON_XPOS								300
#define VOLUME_BUTTON_YPOS											130

#define VOLUME_10_DOWN_BUTTON_XPOS			VOLUME_DOWN_BUTTON_XPOS + 7
#define VOLUME_10_UP_BUTTON_XPOS					VOLUME_UP_BUTTON_XPOS + 7
#define VOLUME_10_BUTTON_YPOS								VOLUME_BUTTON_YPOS + 40

#define APPLY_BUTTON_XPOS													190
#define APPLY_BUTTON_YPOS												BSP_LCD_GetYSize() - 50

#define SET_DEFAULT_BUTTON_XPOS							10
#define SET_DEFAULT_BUTTON_YPOS							BUTTON_YPOS(5) + 10
#define SD_CARD_SAVE_XPOS												BSP_LCD_GetXSize() - SD_CARD_SAVE_WIDTH - 10
#define SD_CARD_SAVE_YPOS												BSP_LCD_GetYSize() - SD_CARD_SAVE_HEIGHT - 10	  


/* Wymiary przycisk�w */
#define BUTTON_WIDTH																	 	100
#define PARAM_BUTTON_WIDTH												35
#define VERSION_BUTTON_WIDTH											55
#define MODE_BUTTON_WIDTH													95
#define SHAPE_BUTTON_WIDTH													55
#define BACK_BUTTON_WIDTH														70
#define ON_OFF_BUTTON_WIDTH												35
#define VOLUME_BUTTON_WIDTH											50
#define APPLY_BUTTON_WIDTH												100
#define DELAY_10_BUTTON_WIDTH										25			
#define SET_DEFAULT_BUTTON_WIDTH						110
#define SD_CARD_SAVE_WIDTH												140


#define BUTTON_HEIGHT       					  									70
#define PARAM_BUTTON_HEIGHT											  30
#define VERSION_BUTTON_HEIGHT										30
#define MODE_BUTTON_HEIGHT													30
#define SHAPE_BUTTON_HEIGHT												30
#define BACK_BUTTON_HEIGHT													40
#define ON_OFF_BUTTON_HEIGHT										ON_OFF_BUTTON_WIDTH
#define VOLUME_BUTTON_HEIGHT											35
#define APPLY_BUTTON_HEIGHT												35
#define DELAY_10_BUTTON_HEIGHT										20
#define SET_DEFAULT_BUTTON_HEIGHT						55
#define SD_CARD_SAVE_HEIGHT										  	35		


/* Polozenie napis�w */
#define TITLE_XPOS																					120
#define TITLE_YPOS																						15
#define GREETING_TITLE_XPOS													80
#define GREETING_TITLE_YPOS												110
#define INFO_XPOS																							20	
#define INFO_YPOS																					225
#define INFO_WIDTH																		  		250
#define INFO_HEIGHT																					40

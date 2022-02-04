/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UMLib
#define __UMLib

#define LED_DAC DAC_CHANNEL_1

#include "stm32f4xx_hal.h"



// LEDS driver
void initializeLEDS(DAC_HandleTypeDef *hdac);
void setLEDS(DAC_HandleTypeDef *hdac, uint32_t n_led);

//halt if failure
void halt();

// UMLib
#endif

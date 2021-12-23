#include "UMLib.h"



/**
  * @brief  disables audio DAC (external) and starts DAC
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */

void initializeLEDS(DAC_HandleTypeDef *hdac){
	// disable audio dac
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);


	// initialize steering wheel leds output
	HAL_DAC_Start(hdac, LED_DAC);
	HAL_DAC_SetValue(hdac, LED_DAC,DAC_ALIGN_12B_R,0);
}

/**
  * @brief  Sets n_leds ON.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  n_led The number of leds to turn on (0-10).
  * @retval None
  */

void setLEDS(DAC_HandleTypeDef *hdac, uint32_t n_led){
	if (n_led > 10){
		assert_param(n_led);
	}

	uint32_t output = n_led * (4000/10);
	HAL_DAC_SetValue(hdac, LED_DAC, DAC_ALIGN_12B_R, output);
}

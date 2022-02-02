#include "SecondMain.h"

//include HAL
#include "stm32f4xx_hal.h"

// Includes for FreeRTOS, don't worry be happy
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

void BlinkTask(void * pvParams) {

  for (;;) {
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

    vTaskDelay(500);
  }
}


int SecondMain(void){


	xTaskCreate(BlinkTask, "Blinking", 150, NULL, 1, NULL);

	// start scheduler, this won't return unless there is an error
	vTaskStartScheduler();


	// endless loop so we never return to that hell of main
	for(;;);
}

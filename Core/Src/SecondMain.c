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

// variables from main.c, for calling HAL functions and so on.
extern DAC_HandleTypeDef hdac;

// variables for debugging
#ifdef DEBUG
extern void initialise_monitor_handles();
#endif

// data packet type
typedef struct dataPacket {
	int timestap = 0;
	int oil_temp = 0;
	int water_temp = 0;
} dataPacket;


// formats input data
void producePacketsTask(void * pvParams){

}

// sends input data
void SendPacketsTask(void * pvParams){

}


// Blink task
void BlinkTask(void * pvParams) {
	int32_t i = 0;

  for (;;) {
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

#ifdef DEBUG
	printf("Toggling %d\n",i);
#endif

	i++;

    vTaskDelay(500);
  }
}


int SecondMain(void){

	// initialize peripherals
	initializeLEDS(&hdac);

	// initialize debugging, if debug release is selected
#ifdef DEBUG
	initialise_monitor_handles();
#endif

	// start blink task
	xTaskCreate(BlinkTask, "Blinking", 128, NULL, 1, NULL);

	// start scheduler, this won't return unless there is an error
	vTaskStartScheduler();
	// endless loop so we never return to that hell of main
	for(;;);
}

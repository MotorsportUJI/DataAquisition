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
typedef struct dataPacket { // 128 bytes
	TickType_t timestap;  // uint32_t
	int oil_temp;
	int water_temp;              // overwritable by default
	int OverWrittable; // 0 if not readed 0xFFFF0000 if thread 1 consumed, 0x0000FFFF if thread2 consumed, 0xFFFFFFFF if it can be overwrited

} dataPacket;

dataPacket* dataTABLE; // holds TABLE_SIZE (elements)

// formats input data
void producePacketsTask(void * pvParams){ // maybe this should be changed so is executed every x ms instead of continous running
	TickType_t timestap = xTaskGetTickCount();

	int index = 0;
	for(;;){

		// check overwrittable
		if (dataTABLE[i].OverWrittable != 0xFFFFFFFF){
			// halt, overrun ocurred (we could reduce speed or something)
#ifdef DEBUG
			printf("TABLE OVERRUN, reduce producer speed");
#endif
			halt();

		}

		//get timestap

		timestap = xTaskGetTickCount(); // this will never be able to collect data faster than 1ms,  we should be sampling at 10ms speed

		// get data




		// store data

		// send data through queue

		// update index
		index = (index + 1) % TABLE_SIZE;
	}



}

// sends input data
void SendPacketsTask(void * pvParams){
	// read queue

	// send data

	// modify overwrittalbe


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

	// allocate 1/2 of the memory as a buffer 512 elements
	dataTABLE = malloc(sizeof(dataPacket) * TABLE_SIZE);

	// populate table with overwritable elements
	dataPacket empty = {0, 0, 0, 0xFFFFFFFF};
	for (int i = 0;i < TABLE_SIZE;i++){
		dataTABLE[i] = empty;
	}

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

#include "UJI/SecondMain.h"

#include <stdlib.h>
#include <string.h>


// variables for debugging (so you can watch easily with live expression)
#ifdef DEBUG

#endif


// sends data to esp32
void SendDataTask(void * pvParams){
	// read queue

	// send data

	// modify overwrittable

	vTaskDelete(NULL);
}


// store data on FAT filesystem
void SaveDataTask(void *pvParams){
	for(;;){

	}
}

// update display and RPM
void updateUI(void *pvParams){

}

//  task for initializing stuff ()
void InitTask(void * pvParams){
	// initialize peripherals
	//leds
	initializeLEDS(&hdac);
	// ADC start in DMA mode
	HAL_ADC_Start_DMA(&hadc1, ADCvalues, sizeof(ADCvalues));


	// exit init task, important, if not hard fault
	vTaskDelete(NULL);

}

int SecondMain(void){


	// start initialize task, (maybe some stuff needs a started scheduler for running)
	xTaskCreate(InitTask, "Initializing Task", 128, NULL, 1, NULL);

	// start scheduler, this won't return unless there is an error
	vTaskStartScheduler();
	// endless loop so we never return to that hell of a main
			halt();
	return 1;
}

// Interruption Callbacks
// ADC error handling (restart ADC)
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc){
#ifdef DEBUG
	asm("BKPT #0");
#endif
	HAL_ADC_Start_DMA(&hadc1, ADCvalues, sizeof(ADCvalues));
}


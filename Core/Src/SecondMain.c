#include "SecondMain.h"

#include <stdlib.h>

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
extern ADC_HandleTypeDef hadc1;


// variables for debugging (so you can watch easily with live expression)
#ifdef DEBUG
int emptySpace = 0;
#endif

// data packet type
typedef struct dataPacket { // 128 bytes
	TickType_t timestap;  // uint32_t
	int oil_temp;
	int water_temp;
	uint16_t ADCvalues[11];
	uint32_t crc;

} dataPacket;

dataPacket* dataTable; // access this with the indexes from the queues

// queue handlers
QueueHandle_t read2uart;
QueueHandle_t uart2read;

// ADC values, get last 4 samples for oversampling
uint16_t ADCvalues[4][11] = {{0,0,0,0,0,0,0,0,0,0,0},
		 	 	 	 	 	 {0,0,0,0,0,0,0,0,0,0,0},
							 {0,0,0,0,0,0,0,0,0,0,0},
							 {0,0,0,0,0,0,0,0,0,0,0}};

// mapping to human understandable names
#define CHAN1i 0
#define CHAN2i 1
#define CHAN3i 2

#define CHAN8i 3
#define CHAN9i 4
#define CHAN11i 5
#define CHAN14i 6
#define CHAN15i 7
#define CPU_TEMPi 8
#define Vrefinti 9
#define Vbati 10


// packs data
void CollectDataTask(void * pvParams){ // run this each 10ms
	for(;;){
		// get timestap
		TickType_t timestap = xTaskGetTickCount(); // this will never be able to collect data faster than 1ms,  we should be sampling at 10ms speed

		// get packet to use
		uint16_t index;
		xQueueReceive(uart2read, &index, portMAX_DELAY);


#ifdef DEBUG
		int emptySpace = uxQueueSpacesAvailable(uart2read);
#endif


		dataPacket* packet = &dataTable[index]; // pointer to packet

		// set timestap
		packet->timestap = timestap;

		// get data
		// gpio reading is atomic
		// gear reading

		// use 21Mhz clock for ADC, each measurement takes 12 +480 cycles, so each 3936 cicles cpu gets interrupted to store value, frequency can be slowed down by using timers/dma, but 0,6% overhead is assumible (assuming that we only expend 25 cycles on interrupt handler, overhead=cycles interrupt(25)/cycles between interrupts(3936+25))
		// copy ADC oversampled values to struct
		for (int i = 0; i < 11; i++){
			int mean = 0;
			for (int j = 0; j < 4; j++){
				mean += ADCvalues[j][i];
			}
			mean = mean / 4;

			packet->ADCvalues[i] = mean;
		}

		// get accelerometer data, spi


		// get CAN data, SPI


		// calculate CRC


		// send data through queue

		xQueueSend(read2uart, &index, portMAX_DELAY);

		// execute this task each 10 miliseconds (period)
		vTaskDelayUntil(&timestap, DATA_AQUISITION_PERIOD);
	}
}

// sends input data
void SendDataTask(void * pvParams){
	// read queue

	// send data

	// modify overwrittable

	vTaskDelete(NULL);
}


// store data on FAT filesystem
void SaveDataTask(void *pvParams){

	vTaskDelete(NULL);
}



// Blink task
void BlinkTask(void * pvParams) {

  for (;;) {
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);


	vTaskDelay(1000);
  }
}

	// main task for initializing stuff
void InitTask(void * pvParams){
	// initialize peripherals
	//leds
	initializeLEDS(&hdac);
	// ADC start in IT mode
	HAL_ADC_Start_IT(&hadc1);
	// ADC_IRQn interrupt is enabled on NVIC by HAL, the interrrupt handler store ADC values on variables, CHECK callback at the bottom

	// create index queues:
	read2uart = xQueueCreate(TABLE_SIZE, sizeof(uint16_t));
	uart2read = xQueueCreate(TABLE_SIZE, sizeof(uint16_t));

	// initialize dataTable
	dataTable = (dataPacket*) malloc(TABLE_SIZE*sizeof(dataPacket));

	// populate dataTable with empty elements (same as formatting to all 0??) and corresponding queue
	dataPacket empty = {};
	for (int i = 0; i < TABLE_SIZE; i++){
		dataTable[i] = empty;
		xQueueSend(uart2read, &i, portMAX_DELAY);

	}

	// start Producer task:
	xTaskCreate(CollectDataTask, "CollectData", 128, NULL, 10, NULL);




	// exit init task, important, if not hard fault
	vTaskDelete(NULL);

}

int SecondMain(void){


	// start initialize task, (maybe some stuff needs a started scheduler for running)
	xTaskCreate(InitTask, "Initializing Task", 128, NULL, 1, NULL);

	// start scheduler, this won't return unless there is an error
	vTaskStartScheduler();
	// endless loop so we never return to that hell of main
			halt();
	return 1;
}

// Interruption Callbacks

// ADC callback
int ADC_callback_index_large = 0;
int ADC_callback_index = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	// write value to according matrix place
	ADCvalues[ADC_callback_index_large][ADC_callback_index] = HAL_ADC_GetValue(&hadc1);
	ADC_callback_index = (ADC_callback_index+1) % 11;
	if (ADC_callback_index == 0){
		ADC_callback_index_large = (ADC_callback_index_large+1) % 4;
	}
}


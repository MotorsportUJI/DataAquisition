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
	int stm32_cpu_temp;
	int oil_temp;
	int water_temp;

} dataPacket;

dataPacket* dataTable; // access this with the indexes from the queues

QueueHandle_t read2uart;
QueueHandle_t uart2read;

// gets data
void CollectDataTask(void * pvParams){ // run this each 10ms
	for(;;){
		// get timestap
		timestap = xTaskGetTickCount(); // this will never be able to collect data faster than 1ms,  we should be sampling at 10ms speed

		// get packet to use
		int index;
		xQueueReceive(uart2read, &index, portMAX_DELAY);

		// print empty espace if too low: (producing faster than consuming)
#ifdef DEBUG
		int e = uxQueueSpacesAvailable(uart2read);
		if (e > 400){
			printf("uart2read queue getting empty, %d empty spaces, consumers not fast enough \n",e);
		}
#endif


		dataPacket* packet = &dataTable[index]; // pointer to packet

				// set timestap
		dataPacket->timestap=timestap;


		// get data
			// gpio reading is atomic
				// gear reading
				// gpio reading

			// check ADC dma readiness
			// get values from dma ADC (pack)
			// make dma ADC requests

		// get accelerometer data, spi
			// check accelerometer readiness
			// get values from accelerometer (pack)
			// make dma accelerometer requests

		// get CAN data, SPI
			// check CAN data readiness
			// get values from CAN (pack)
			// make dma CAN requests





		// store data

		// send data through queue


		xQueueSend(read2uart, &index, portMAX_DELAY);
		vTaskDelayUntil(timestap, 10);
	}
}

	// sends input data
	void SendDataTask(void * pvParams){
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

	// main task for initializing stuff
	void InitTask(void * pvParams){
		// initialize peripherals
			initializeLEDS(&hdac);


			// create index queues:
			read2uart = xQueueCreate(TABLE_SIZE, sizeof(uint16_t));
			uart2read = xQueueCreate(TABLE_SIZE, sizeof(uint16_t));

			// initialize dataTable
			dataTable = (dataPacket*) malloc(TABLE_SIZE*sizeof(dataPacket));

			// populate dataTable and corresponding queue
			dataPacket empty = {0,0,0};
			for (int i = 0; i < TABLE_SIZE; i++){
				dataTable = empty[i];
				xQueueSend(uart2read, &i, portMAX_DELAY);

			}

			// start Producer task:
			xTaskCreate(CollectDataTask, "CollectData", 128, NULL, 10, NULL);
	}
}
int SecondMain(void){


	// initialize debugging, if debug release is selected
#ifdef DEBUG
	initialise_monitor_handles();
#endif

	// start blink task
	xTaskCreate(InitTask, "Initializing Task", 128, NULL, 1, NULL);

	// start scheduler, this won't return unless there is an error
	vTaskStartScheduler();
	// endless loop so we never return to that hell of main
#ifdef DEBUG
			printf("ERROR STARTING SCHEDULER");
#endif
			halt();
}

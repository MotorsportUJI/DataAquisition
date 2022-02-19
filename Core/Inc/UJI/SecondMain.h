/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SecondMain
#define __SecondMain

#include <stdbool.h>


#include "UMLib.h"
#include "main.h"

//include HAL
#include "stm32f4xx_hal.h"

// Includes for FreeRTOS, don't worry be happy
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

// data packet type, put items with same size together to prevent padding
typedef struct dataPacket { // 40 bytes
	TickType_t timestap;  // uint32_t
	uint32_t oil_temp;
	uint32_t water_temp;
	uint16_t ADCvalues[11];
	uint16_t padding; // padding for crc calculation (alligned to 32 bits), modify when adding new elements to struct (we could delete this and the compiler will take care for us, but I think that this makes it more visual)
	uint32_t crc;

} dataPacket;


// ADC TABLES, get last (number of rows) samples for oversampling (each column is a channel)
uint16_t ADCvalues[4][11];

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


// variables from main.c, for calling HAL functions and so on.
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;
extern CRC_HandleTypeDef hcrc;

#define DATA_AQUISITION_PERIOD 10

int SecondMain();
// SecondMain
#endif

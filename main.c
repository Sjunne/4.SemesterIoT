/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

#include <stdio.h>
#include <avr/io.h>
#include "LoRaWANHandler.h"
//#include <ATMEGA_FreeRTOS.h>
#include <task.h>

//#include <lora_driver.h>
//#include <status_leds.h>

//#include <stdio_driver.h>
#include <serial.h>
#include <string.h>
#include <mh_z19.h>
#include <hih8120.h>
#include "SharedDataQueue.h"
#include "TaskHandler.h"
#include "CO2Handler.h"
#include "TempHumHandler.h"


void lora_handler_initialise(UBaseType_t lora_handler_task_priority);


/*-----------------------------------------------------------*/
void initialiseSystem()
{
printf("Program Started!!\n");
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	
	// Initialize queues
	initializeCO2Queue();
	initializeTempHumQueues();
	initializeSharedDataQueue();

	// Initializing co2 sensor
	mh_z19_initialise(ser_USART3);
	hih8120_initialise();



	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	// KAN IKKE KØRE MED DENNE METODE
	MessageBufferHandle_t downlinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);

}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program Started!!\n");
	startTasks();

	/* Replace with your application code */
	while (1)
	{
	}
}


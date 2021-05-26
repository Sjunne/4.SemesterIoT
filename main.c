/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>

#include <stdio_driver.h>
#include <serial.h>
#include <string.h>
#include <mh_z19.h>
#include "SharedDataQueue.h"
#include "TaskHandler.h"

 // Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

<<<<<<< Updated upstream


=======
>>>>>>> Stashed changes
/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
<<<<<<< Updated upstream
	lora_driver_initialise(1, NULL);
=======
	downlinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t));
	lora_driver_initialise(ser_USART1, downlinkMessageBufferHandle);
>>>>>>> Stashed changes
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
	
	// Initializing co2 sensor
	mh_z19_initialise(ser_USART3);
	
	
	// Initialize queues
	initializeQueues();
	initializeSharedDataQueue();
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program Started!!\n");
	startTasks();
<<<<<<< Updated upstream

	/* Replace with your application code */
=======
>>>>>>> Stashed changes
	while (1)
	{
	}
}


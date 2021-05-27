/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

#include <stdio.h>
#include <avr/io.h>
#include "../networkdrivers/header/LoRaWANHandler.h"
#include <task.h>

#include <serial.h>
#include <string.h>
#include "../data/header/SharedDataQueue.h"
#include "../tasks/header/TaskHandler.h"
#include "../measuredrivers/header/CO2Handler.h"
#include "../measuredrivers/header/TempHumHandler.h"
#include "../servodrivers/header/ServoHandler.h"
#include "../semaphore/header/testOutprint.h"


void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void initialiseSystem()
{
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
	// Initializing temperature & humidity sensor
	hih8120_initialise();
	// Initializing servo
	rc_servo_initialise();



	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	downlinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);
	lora_driver_initialise(ser_USART1, downlinkMessageBufferHandle);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);

}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	
	initializeSemaphore();
	
	puts("Program started!");
	
	startTasks();
	while (1)
	{
	}
}



/*
 * TaskHandlerImpl.c
 *
 * Created: 28-04-2021 13:08:00
 *  Author: kawaz
 */

#include "../tasks/header/TaskHandler.h" 
#include "../semaphore/header/testOutprint.h"

// define Tasks
void takeMeasure( void *pvParameters );
void enqueueMeasures( void *pvParameters );


void setupTasks(void) // Can only setup 2 tasks, memory problems?
{
	
	xTaskCreate(
	takeMeasure
	,  "takeMeasure"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  0 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

	xTaskCreate(
	enqueueMeasures
	,  "enqueueMeasures"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );	
	
}


void takeMeasure(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 3000/portTICK_PERIOD_MS; // 60 s

	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
		
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		tempHumMeasure();	
		co2Measure(); // Call CO2HandlerImpl to take measure
	}
}

void enqueueMeasures(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 30000/portTICK_PERIOD_MS; // 65 s

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		enqueueSharedData(); // Call SharedDataQueueImpl to enqueue the measures
	}
}


void startTasks()
{
	setupTasks();
	
	sprintf(printstring, "Heapsize: %d \n", xPortGetFreeHeapSize());
	//test_outprint(printstring);
	
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
}




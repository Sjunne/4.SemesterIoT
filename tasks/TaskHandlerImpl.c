
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
	const TickType_t xFrequency = 60000/portTICK_PERIOD_MS; // 60 s

	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
		
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		tempHumMeasure(); // Call TempHumHandler to take measure
		co2Measure(); // Call CO2Handler to take measure
	}
}

void enqueueMeasures(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 600000/portTICK_PERIOD_MS; // 10 minutes

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		enqueueSharedData(); // Call SharedDataQueue to enqueue the measures
	}
}


void startTasks()
{
	setupTasks();
	
	sprintf(printstring, "Heapsize: %d \n", xPortGetFreeHeapSize()); // Check to see if there is enough heap
	//test_outprint(printstring);
	
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
}




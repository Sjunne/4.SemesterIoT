
/*
 * TaskHandlerImpl.c
 *
 * Created: 28-04-2021 13:08:00
 *  Author: kawaz
 */

#include "TaskHandler.h" 

// define Tasks
void taskMeasureCO2( void *pvParameters );
void queueCO2( void *pvParameters );


void setupTasks(void)
{
	
	xTaskCreate(
	taskMeasureCO2
	,  "CO2MeasureTask"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  0 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

	xTaskCreate(
	queueCO2
	,  "queueCO2"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );	
}


void taskMeasureCO2(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 60000/portTICK_PERIOD_MS; // 60 s

	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
		
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		tempHumMeasure();	
		co2Measure(); // Call CO2HandlerImpl to take measure
	}
}

void queueCO2(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 65000/portTICK_PERIOD_MS; // 65 s

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
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
}




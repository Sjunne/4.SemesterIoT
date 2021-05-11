
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
void dequeueCO2( void *pvParameters );


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
	
	xTaskCreate(
	dequeueCO2
	,  "dequeueCO2"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}


void taskMeasureCO2(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 7000/portTICK_PERIOD_MS; // 2500 ms

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
	const TickType_t xFrequency = 8000/portTICK_PERIOD_MS; // 5500 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		enqueueSharedData(); // Call SharedDataQueueImpl to enqueue the measures
	}
}

void dequeueCO2(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 12000/portTICK_PERIOD_MS; // 10 s
	
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	//SharedData_t sharedDataArray[15];
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		int arrayIndex = 0;

		
		// FOR WAITING:
		TickType_t xLastWakeTime2;
		const TickType_t xFrequency2 = 5/portTICK_PERIOD_MS;
		xLastWakeTime2 = xTaskGetTickCount();


		SharedData_t sharedData;
		sharedData = dequeueSharedData();
		xTaskDelayUntil( &xLastWakeTime2, xFrequency2 );
		printf("DEQUEUE: humidity: %d, co2: %d, Temp: %d \n", sharedData->humidity, sharedData->co2, sharedData->temperature);


		//memset(sharedData, 0, sizeof(SharedData));

		//brug semaphore
		//while(flag) {
		//	dequeueData = dequeueSharedData(); // Call SharedDataQueueImpl to dequeue the measures
		//	if (dequeueData->returnCode == OK)
		//	{
		//		printf("Dequeueing OK \n");
		//		sharedDataArray[arrayIndex] = dequeueData->sharedData;
		//	}
		//	else if(dequeueData->returnCode == ENDOFQUEUE)
		//	{
		//		printf("Dequeueing ENDOFQUEUE \n");
		//		for(int i = 0; i < (sizeof(sharedDataArray) / sizeof(SharedData)); i++)
		//		{
		//			//printf("i is: %d \n", i);
		//			//printf("Sending to lorawan: %d \n", sharedDataArray[i]->co2);
		//		}
		//		flag = false;
		//		// memset(sharedDataArray, 0, sizeof sharedDataArray);  JUST TO CLEAR MEMORY
		//	}
		//	arrayIndex++;
		//}
		
	}
}

void startTasks()
{
	setupTasks();
	vTaskStartScheduler(); // Initialize and run the freeRTOS scheduler. Execution should never return from here.
}




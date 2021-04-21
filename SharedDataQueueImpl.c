/*
 * SharedDataQueue.c
 *
 * Created: 14-04-2021 15:38:50
 *  Author: Sjunn
 */ 
#include "SharedDataQueue.h"

QueueHandle_t xQueueShared;
SharedData myDataQueue;
SharedData receiveSharedData;
SharedDataWithReturnCode sharedDataWithReturnCode;
BaseType_t xStatus;

void initializeSharedDataQueue()
{
	xQueueShared = xQueueCreate(15, sizeof(SharedData));
}

void enqueueSharedData()
{
	uint16_t co2 = dequeueCO2Measure();
	//uint16_t temp = dequeueTempMeasure();
	//uint16_t light = dequeueLightMeasure();
	printf("Enqueuing Shared Data CO2: %d \n" , co2);

	// TODO:
	SharedData_t shared = pvPortMalloc(sizeof(SharedData)); //Allocation memory to SharedData
	shared->co2 = co2;
	
	xQueueSend(xQueueShared, (void*)&shared, portMAX_DELAY);
}

SharedDataWithReturnCode_t dequeueSharedData()
{
	SharedData_t received = &receiveSharedData;
	
	SharedDataWithReturnCode_t sharedDataToReturn = &sharedDataWithReturnCode;
	
	
	xStatus = xQueueReceive(xQueueShared, &received, (TickType_t) 10);
	
	if (xStatus == pdPASS){
		sharedDataToReturn->returnCode = OK;
		sharedDataToReturn->sharedData = received;
	}
	else {
		sharedDataToReturn->returnCode = ENDOFQUEUE;
	}
	
	vPortFree(received);
	return sharedDataToReturn;
}

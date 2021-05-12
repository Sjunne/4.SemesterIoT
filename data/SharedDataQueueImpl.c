/*
 * SharedDataQueue.c
 *
 * Created: 14-04-2021 15:38:50
 *  Author: Sjunn
 */ 
#include "../data/header/SharedDataQueue.h"

QueueHandle_t xQueueShared;
SharedData_t myDataQueue;
SharedData receiveSharedData;
BaseType_t xStatus;
SharedData enqueueSharedData2;

void initializeSharedDataQueue()
{
	xQueueShared = xQueueCreate(5, sizeof(SharedData));
}

void enqueueSharedData()
{
	uint16_t co2 = dequeueCO2Measure();
	int16_t temp = dequeueTempMeasure();
	uint16_t hum = dequeueHumidityMeasure();

	SharedData_t shared = &enqueueSharedData2;
	shared->co2 = co2;
	shared->temperature = temp;
	shared->humidity = hum;
		
	TickType_t xLastWakeTime2;
	const TickType_t xFrequency2 = 5/portTICK_PERIOD_MS;
	xLastWakeTime2 = xTaskGetTickCount();


	xTaskDelayUntil( &xLastWakeTime2, xFrequency2 );
	printf("ENQUEUE: humidity: %d, co2: %d, Temp: %d \n", shared->humidity, shared->co2, shared->temperature);

	xQueueSend(xQueueShared, (void*)&enqueueSharedData2, portMAX_DELAY);

}

SharedData_t dequeueSharedData() {
	SharedData_t data = &receiveSharedData;
	xQueueReceive(xQueueShared, &receiveSharedData, (TickType_t) 10);
	return data;
}

/*
 * SharedDataQueue.c
 *
 * Created: 14-04-2021 15:38:50
 *  Author: Sjunn
 */ 
#include "../data/header/SharedDataQueue.h"

QueueHandle_t xQueueShared;

SharedData receiveSharedData;
SharedData sharedData;

void initializeSharedDataQueue()
{
	xQueueShared = xQueueCreate(5, sizeof(SharedData));
}

void enqueueSharedData()
{
	/*int counter = 0;
	uint16_t co2;
	float divider = 10;
	float res = 0.0;
	while (counter < 10) {
		co2 += dequeueCO2Measure();
		counter++;
	}
	res = (co2 / divider);
	printf("c02 som float %.2f \n", res);
	co2 = (uint16_t) res;
	*/
	
	uint16_t co2 = dequeueCO2Measure();
	int16_t temp = dequeueTempMeasure();
	uint16_t hum = dequeueHumidityMeasure();

	SharedData_t shared = &sharedData;
	shared->co2 = co2;
	shared->temperature = temp;
	shared->humidity = hum;

	printf("ENQUEUE: humidity: %d, co2: %d, Temp: %d \n", shared->humidity, shared->co2, shared->temperature);

	xQueueSend(xQueueShared, (void*)&sharedData, portMAX_DELAY);

}

SharedData_t dequeueSharedData() {
	SharedData_t data = &receiveSharedData;
	xQueueReceive(xQueueShared, &receiveSharedData, (TickType_t) 10);
	return data;
}


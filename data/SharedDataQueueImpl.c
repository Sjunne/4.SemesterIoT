/*
 * SharedDataQueue.c
 *
 * Created: 14-04-2021 15:38:50
 *  Author: Sjunn
 */ 
#include "../data/header/SharedDataQueue.h"
#include "../semaphore/header/testOutprint.h"

QueueHandle_t xQueueShared;

SharedData receiveSharedData;
SharedData sharedData;

void initializeSharedDataQueue()
{
	xQueueShared = xQueueCreate(5, sizeof(SharedData));
}

void enqueueSharedData()
{
	TickType_t xDelay = 25 / portTICK_PERIOD_MS;
			
	int counter = 0;
	
	uint16_t co2 = 0;
	int16_t temp = 0;
	uint16_t hum = 0;
	
	int co2res = 0;
	int tempres = 0;
	int humres = 0;
	while (counter < 10) {
		co2 += dequeueCO2Measure();
		vTaskDelay(xDelay);
		temp += dequeueTempMeasure();
		vTaskDelay(xDelay);
		hum += dequeueHumidityMeasure();
		vTaskDelay(xDelay);
		counter++;
	}
	co2res = (co2 / (counter + 1));
	co2 = (uint16_t) co2res;
	
	tempres = (temp / (counter + 1));
	temp = (int16_t) tempres;
	
	humres = (hum / (counter + 1));
	hum = (uint16_t) humres;

	SharedData_t shared = &sharedData;
	shared->co2 = co2;
	shared->temperature = temp;
	shared->humidity = hum;
	
	sprintf(printstring, "ENQUEUE: humidity: %d, co2: %d, Temp: %d \n", hum, co2, temp);
	test_outprint(printstring);

	xQueueSend(xQueueShared, (void*)&sharedData, portMAX_DELAY);

}

SharedData_t dequeueSharedData() {
	SharedData_t data = &receiveSharedData;
	xQueueReceive(xQueueShared, &receiveSharedData, (TickType_t) 10);
	return data;
}


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
	TickType_t xDelay = 25/portTICK_PERIOD_MS;
	int counter = 0;
	bool flag = false;
	
	uint16_t co2 = 0;
	int16_t temp = 0;
	uint16_t hum = 0;
	
	int co2res = 0;
	int tempres = 0;
	int humres = 0;
	while (counter < 10) {
		int dequeuemeasure;
		
		dequeuemeasure = dequeueCO2Measure();
		if (dequeuemeasure == 0)
		{
			flag = true;
		}
		else {
			co2 += dequeuemeasure;
			vTaskDelay(xDelay);
		}
		
		dequeuemeasure = dequeueTempMeasure();
		if (dequeuemeasure == 0)
		{
			flag = true;
		}
		else {
		temp += dequeuemeasure;
		vTaskDelay(xDelay);
		}
		
		dequeuemeasure = dequeueHumidityMeasure();
		if (dequeuemeasure == 0)
		{
			flag = true;
		}
		else {
		hum += dequeuemeasure;
		vTaskDelay(xDelay);
		}
		counter++;
	}
	
	SharedData_t shared = &sharedData;
		
	if (flag)
	{
		shared->co2 = 0;
		shared->temperature = 0;
		shared->humidity = 0;
	}
	else {
		// Average calc of co2
		co2res = (co2 / (counter + 1));
		co2 = (uint16_t) co2res;
		
		// Average calc of temperature
		tempres = (temp / (counter + 1));
		temp = (int16_t) tempres;
		
		// Average calc of humidity
		humres = (hum / (counter + 1));
		hum = (uint16_t) humres;

		shared->co2 = co2;
		shared->temperature = temp;
		shared->humidity = hum;
	}

	printf("ENQUEUE: humidity: %d, co2: %d, Temp: %d \n", shared->humidity, shared->co2, shared->temperature);

	xQueueSend(xQueueShared, (void*)&sharedData, portMAX_DELAY);

}

SharedData_t dequeueSharedData() {
	SharedData_t data = &receiveSharedData;
	xQueueReceive(xQueueShared, &receiveSharedData, (TickType_t) 10);
	return data;
}


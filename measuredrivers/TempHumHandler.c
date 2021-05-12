
/*
 * TempHumHandler.c
 *
 * Created: 05-05-2021 11:31:16
 *  Author: kawaz
 */ 

#include "../measuredrivers/header/TempHumHandler.h"

QueueHandle_t xQueueTemp;
QueueHandle_t xQueueHum;


void initializeTempHumQueues()
{
	xQueueTemp = xQueueCreate(5, sizeof(int16_t));
	xQueueHum = xQueueCreate(5, sizeof(uint16_t));
}

void enqueueTemperature(int16_t temp){
	xQueueSend(xQueueTemp, (void*)&temp, portMAX_DELAY);
}

void enqueueHumidity(uint16_t hum){
	xQueueSend(xQueueHum, (void*)&hum, portMAX_DELAY);	
}


void tempHumMeasure()
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 5/portTICK_PERIOD_MS; 
	const TickType_t xFrequency2 = 50/portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();

	if(HIH8120_OK != hih8120_wakeup())
	{
		printf("wakeup gik galt");
	}
	xTaskDelayUntil( &xLastWakeTime, xFrequency2 );
	if(HIH8120_OK != hih8120_measure())
	{
		printf("measure gik galt");
	}
	xTaskDelayUntil( &xLastWakeTime, xFrequency );
	
	uint16_t humidity = hih8120_getHumidity();
	int16_t temp = hih8120_getTemperature();
	
	enqueueHumidity(humidity);
	enqueueTemperature(temp);
}


int16_t dequeueTempMeasure(){
	int16_t temp;
	
	xQueueReceive(xQueueTemp, &temp, 1500);
	return temp;
}

uint16_t dequeueHumidityMeasure(){
	uint16_t hum;
	
	xQueueReceive(xQueueHum, &hum, 1500);
	return hum;
}
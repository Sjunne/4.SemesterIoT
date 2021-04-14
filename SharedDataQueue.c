/*
 * SharedDataQueue.c
 *
 * Created: 14-04-2021 15:38:50
 *  Author: Sjunn
 */ 
#include "SharedDataQueue.h"
#include <stdio.h>

QueueHandle_t xQueueShared;



typedef struct SharedData {
	uint16_t co2;
	//uint16_t temp;
	//uint16_t light;
	//timestamp
} SharedData;

void initializeSharedDataQueue()
{
	xQueueShared = xQueueCreate(15, sizeof(SharedData));
}

void enqueueSharedData()
{
	uint16_t co2 = dequeueCO2Measure();
	//uint16_t temp = dequeueTempMeasure();
	//uint16_t light = dequeueLightMeasure();
	printf("CO2 PPM: %d slut" , co2);

	//Det går galt her
	SharedData_t shared = calloc(sizeof(SharedData), 1);
	shared->co2 = co2;
	
	xQueueSend(xQueueShared, (void*)&shared, portMAX_DELAY);
}


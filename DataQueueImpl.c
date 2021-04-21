/*
 * SharedDataQueueImpl.c
 *
 * Created: 14-04-2021 14:27:14
 *  Author: Sjunn
 */ 

#include "DataQueue.h"

QueueHandle_t xQueueCO2;

void initializeQueues() {
	xQueueCO2 = xQueueCreate(15, sizeof(uint16_t));
}

void enqueueCO2Measure(uint16_t ppm){
	xQueueSend(xQueueCO2, (void*)&ppm, portMAX_DELAY);
}

uint16_t dequeueCO2Measure(){
	uint16_t ppm;
	
	xQueueReceive(xQueueCO2, &ppm, 1500);
	return ppm;
}

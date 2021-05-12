/*
 * CO2HandlerImpl.c
 *
 * Created: 14-04-2021 14:05:30
 *  Author: Sjunn
 */ 

#include "CO2Handler.h"

QueueHandle_t xQueueCO2;

void initializeCO2Queue() {
	xQueueCO2 = xQueueCreate(5, sizeof(uint16_t));
}


void enqueueCO2Measure(uint16_t ppm){
	xQueueSend(xQueueCO2, (void*)&ppm, portMAX_DELAY);
}

void co2Measure() {
	mh_z19_returnCode_t rc;
	
	rc = mh_z19_takeMeassuring();
	if (rc == MHZ19_OK)
	{
		mh_z19_injectCallBack(enqueueCO2Measure);
	}
	else
	{
		puts("det gik galt");
	}
}


uint16_t dequeueCO2Measure(){
	uint16_t ppm;
	
	xQueueReceive(xQueueCO2, &ppm, 1500);
	return ppm;
}
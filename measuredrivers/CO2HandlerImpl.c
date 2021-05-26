/*
 * CO2HandlerImpl.c
 *
 * Created: 14-04-2021 14:05:30
 *  Author: Sjunn
 */ 

#include "../measuredrivers/header/CO2Handler.h"
#include "../semaphore/header/testOutprint.h"

QueueHandle_t xQueueCO2;

void initializeCO2Queue() {
	xQueueCO2 = xQueueCreate(10, sizeof(uint16_t));
}


void enqueueCO2Measure(uint16_t ppm){
	sprintf(printstring, "Putting CO2: %d in the queue \n", ppm);
	test_outprint(printstring);
	
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
		puts("Det gik galt");
	}
}


uint16_t dequeueCO2Measure(){
	uint16_t ppm;
	
	xQueueReceive(xQueueCO2, &ppm, 1500);
	
	sprintf(printstring, "Removing CO2: %d from the queue \n", ppm);
	test_outprint(printstring);

	return ppm;
}
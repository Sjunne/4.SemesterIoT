
/*
 * TempHumHandler.c
 *
 * Created: 05-05-2021 11:31:16
 *  Author: kawaz
 */ 

#include "../measuredrivers/header/TempHumHandler.h"
#include "../semaphore/header/testOutprint.h"

QueueHandle_t xQueueTemp;
QueueHandle_t xQueueHum;


void initializeTempHumQueues(){
	// Creating queues with 10 positions
	xQueueTemp = xQueueCreate(10, sizeof(int16_t));
	xQueueHum = xQueueCreate(10, sizeof(uint16_t));
}

void enqueueTemperature(int16_t temp){
	sprintf(printstring, "Putting temperature: %d in the queue \n", temp);
	test_outprint(printstring);
	
	// Sending measure to queue
	xQueueSend(xQueueTemp, (void*)&temp, portMAX_DELAY);
}

void enqueueHumidity(uint16_t hum){
	sprintf(printstring, "Putting humidity: %d in the queue \n", hum);
	test_outprint(printstring);
	
	// Sending measure to queue	
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
		puts("HIH8120 Wakeup gik galt");
	}
	
	//Short delay so the driver has time to wakeup
	xTaskDelayUntil( &xLastWakeTime, xFrequency2 );
	
	if(HIH8120_OK != hih8120_measure())
	{
		puts("Humidity & temperature measure went wrong");
		
		// Enqueueing 0=ERROR CODE, if measure went wrong
		enqueueHumidity(0);
		enqueueTemperature(0);
	}
	xTaskDelayUntil( &xLastWakeTime, xFrequency );
	
	// Call to get value from driver
	uint16_t humidity = hih8120_getHumidity();
	int16_t temp = hih8120_getTemperature();
	
	// Helps to make sure that the first measurevalues is discarded, as they're always max value
	if (humidity > 95 && temp > 100) {
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
			
		humidity = hih8120_getHumidity();
		temp = hih8120_getTemperature();
	}
	
	// Enqueueing the two measures
	enqueueHumidity(humidity);
	enqueueTemperature(temp);
}


int16_t dequeueTempMeasure(){
	int16_t temp;
	
	//Receiving measure from queue
	xQueueReceive(xQueueTemp, &temp, 1500);
	
	sprintf(printstring, "Removing temperature: %d from the queue \n", temp);
	test_outprint(printstring);
	
	return temp;
}

uint16_t dequeueHumidityMeasure(){
	uint16_t hum;
	
	//Receiving measure from queue	
	xQueueReceive(xQueueHum, &hum, 1500);
	
	sprintf(printstring, "Removing humidity: %d from the queue \n", hum);
	test_outprint(printstring);
		
	return hum;
}
/*
 * SharedDataQueue.c
 *
 * Created: 14-04-2021 15:38:50
 *  Author: Sjunn
 */ 
#include "../data/header/SharedDataQueue.h"

// Declaring variables for static memory allocation
QueueHandle_t xQueueShared;
SharedData receiveSharedData;
SharedData sharedData;

void initializeSharedDataQueue()
{
	// Creating queue with 5 positions
	xQueueShared = xQueueCreate(5, sizeof(SharedData));
}

void enqueueSharedData()
{
	TickType_t xDelay = 25/portTICK_PERIOD_MS;
	int counter = 0;
	
	// Declaring flag for corrupt data
	bool flag = false;
	
	// Declaring and initializing values to insert into struct
	uint16_t co2 = 0;
	int16_t temp = 0;
	uint16_t hum = 0;
	
	// Declaring and initializing ints to hold result from average calculation
	int co2res = 0;
	int tempres = 0;
	int humres = 0;
	
	// Loop for dequeuing 10 measures 
	while (counter < 10) {
		// Declaring integer to hold dequeue value
		int dequeuemeasure;
		
		// Dequeuing 
		dequeuemeasure = dequeueCO2Measure();
		
		// Check to see if measure = 0 which is indicator of corrupt measure
		if (dequeuemeasure == 0)
		{
			// Setting flag to true indicating that we discard all data
			flag = true;
		}
		// If the measure is not 0 we increment co2 with dequeue value
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
		
	// If data is corrupt, putting 0 for all measures into struct
	if (flag)
	{
		shared->co2 = 0;
		shared->temperature = 0;
		shared->humidity = 0;
	}
	else {
		// Average calc of co2
		co2res = (co2 / counter);
		co2 = (uint16_t) co2res;
		
		// Average calc of temperature
		tempres = (temp / counter);
		temp = (int16_t) tempres;
		
		// Average calc of humidity
		humres = (hum / counter);
		hum = (uint16_t) humres;

		//Inserting values into struct
		shared->co2 = co2;
		shared->temperature = temp;
		shared->humidity = hum;
	}

	printf("ENQUEUE: humidity: %d, co2: %d, Temp: %d \n", shared->humidity, shared->co2, shared->temperature);

	// Inserting shared data into queue
	xQueueSend(xQueueShared, (void*)&sharedData, portMAX_DELAY);

}

SharedData_t dequeueSharedData() {
	// Declaring pointer, referencing address of class variable
	SharedData_t data = &receiveSharedData;
	
	//Receiving shared data from queue	
	xQueueReceive(xQueueShared, &receiveSharedData, (TickType_t) 10);
	return data;
}


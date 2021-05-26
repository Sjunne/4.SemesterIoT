/*
 * testOutprintImpl.c
 *
 * Created: 26-05-2021 10:35:55
 *  Author: benja
 */ 

#include "../semaphore/header/testOutprint.h"


void test_outprint(char* outprint) {
	//TickType_t xDelay = 50 / portTICK_PERIOD_MS;
	//vTaskDelay(xDelay);
	
	//TickType_t xLastWakeTime;
	//const TickType_t xFrequency = 50/portTICK_PERIOD_MS;
	//xLastWakeTime = xTaskGetTickCount();
	//xTaskDelayUntil( &xLastWakeTime, xFrequency );
	
	if( xSemaphoreTake( xSemaphore, ( TickType_t ) 100 ) == pdTRUE )
	{
		printf(outprint);
		xSemaphoreGive( xSemaphore );
	}
	else
	{ 
		puts("The semaphore was already taken");
	}
	
}

void initializeSemaphore(){
	xSemaphore = xSemaphoreCreateBinary();
	xSemaphoreGive(xSemaphore);
}
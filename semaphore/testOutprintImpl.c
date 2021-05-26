/*
 * testOutprintImpl.c
 *
 * Created: 26-05-2021 10:35:55
 *  Author: benja
 */ 

#include "../semaphore/header/testOutprint.h"


void test_outprint(char* outprint) {
	if( xSemaphoreTake( xSemaphore, ( TickType_t ) 100 ) == pdTRUE )
	{
		printf(outprint);
		//TickType_t xDelay = 100 / portTICK_PERIOD_MS;
		//vTaskDelay(xDelay);
		
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
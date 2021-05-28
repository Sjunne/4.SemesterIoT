/*
 * testOutprintImpl.c
 *
 * Created: 26-05-2021 10:35:55
 *  Author: benja
 */ 

#include "../semaphore/header/testOutprint.h"


void test_outprint(char* outprint) {
	// Check to see if semaphore is free
	if( xSemaphoreTake( xSemaphore, ( TickType_t ) 100 ) == pdTRUE )
	{
		// Protecting critical section
		printf(outprint);		
		
		// Releasing semaphore again
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
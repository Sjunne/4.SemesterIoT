
/*
 * SharedDataQueue.h
 *
 * Created: 14-04-2021 15:18:09
 *  Author: Sjunn
 */

#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <queue.h>
#include "CO2Handler.h"
#include "TempHumHandler.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct SharedData {
	uint16_t co2;
	int16_t temperature;
	uint16_t humidity;
	//uint16_t timestamp;
} SharedData;

typedef struct SharedData* SharedData_t;




void initializeSharedDataQueue();
void enqueueSharedData();
SharedData_t dequeueSharedData();


/*
 * SharedDataQueue.h
 *
 * Created: 14-04-2021 15:18:09
 *  Author: Sjunn
 */

#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <queue.h>
#include "../measuredrivers/header/CO2Handler.h"
#include "../measuredrivers/header/TempHumHandler.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct SharedData {
	uint16_t co2;
	int16_t temperature;
	uint16_t humidity;
} SharedData;

typedef struct SharedData* SharedData_t;




void initializeSharedDataQueue();
void enqueueSharedData();
SharedData_t dequeueSharedData();

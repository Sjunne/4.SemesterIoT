
/*
 * SharedDataQueue.h
 *
 * Created: 14-04-2021 15:18:09
 *  Author: Sjunn
 */

#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <queue.h>
#include "DataQueue.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum ReturnCode {
	OK,
	ENDOFQUEUE
} ReturnCode;

typedef struct SharedData {
	uint16_t co2;
	//uint16_t temp;
	//uint16_t light;
	//timestamp
} SharedData;


typedef struct SharedData* SharedData_t;

typedef struct SharedDataWithReturnCode {
	ReturnCode returnCode;
	SharedData_t sharedData;
} SharedDataWithReturnCode;

typedef struct SharedDataWithReturnCode* SharedDataWithReturnCode_t;

void initializeSharedDataQueue();
void enqueueSharedData();
SharedDataWithReturnCode_t dequeueSharedData();

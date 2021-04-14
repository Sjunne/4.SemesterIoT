
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

typedef struct SharedData* SharedData_t;

void initializeSharedDataQueue();
void enqueueSharedData();
SharedData_t dequeueSharedData();

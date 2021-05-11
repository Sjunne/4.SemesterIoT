
/*
 * TempHumHandler.h
 *
 * Created: 05-05-2021 11:28:34
 *  Author: kawaz
 */ 

#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <queue.h>
#include <hih8120.h>


#include <stdlib.h>

void initializeTempHumQueues();

void tempHumMeasure();

int16_t dequeueTempMeasure();
uint16_t dequeueHumidityMeasure();


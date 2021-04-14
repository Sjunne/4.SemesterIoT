/*
 * SharedDataQueue.h
 *
 * Created: 14-04-2021 14:24:18
 *  Author: Sjunn
 */ 

#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <queue.h>

void initializeQueues();

void enqueueCO2Measure(uint16_t ppm);
uint16_t dequeueCO2Measure();


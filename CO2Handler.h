/*
 * CO2Handler.h
 *
 * Created: 14-04-2021 13:57:40
 *  Author: Sjunn
 */ 

#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <queue.h>
#include <mh_z19.h>

#include <stdlib.h>

void initializeCO2Queue();
void co2Measure();
uint16_t dequeueCO2Measure();


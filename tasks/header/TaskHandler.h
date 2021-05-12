
/*
 * TaskHandler.h
 *
 * Created: 28-04-2021 13:04:13
 *  Author: kawaz
 */ 

#pragma once

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include "../measuredrivers/header/TempHumHandler.h"

#include "../measuredrivers/header/CO2Handler.h"
#include "../data/header/SharedDataQueue.h"

void setupTasks(void);

void startTasks();

void takeMeasure(void *pvParameters);
void enqueueMeasures(void *pvParameters);



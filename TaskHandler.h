
/*
 * TaskHandler.h
 *
 * Created: 28-04-2021 13:04:13
 *  Author: kawaz
 */ 

#pragma once

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include "TempHumHandler.h"

#include "CO2Handler.h"
#include "SharedDataQueue.h"
#include "TaskHandler.h"

void setupTasks(void);

void startTasks();

void taskMeasureCO2(void *pvParameters);

void taskMeasureTemp(void *pvParameters);

void taskMeasureHum(void *pvParameters);


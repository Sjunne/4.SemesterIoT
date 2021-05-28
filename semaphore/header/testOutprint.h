/*
 * testOutprint.h
 *
 * Created: 26-05-2021 10:35:26
 *  Author: benja
 */ 
#pragma once
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

SemaphoreHandle_t xSemaphore;
char printstring[100];


void test_outprint(char* outprint);

void initializeSemaphore();
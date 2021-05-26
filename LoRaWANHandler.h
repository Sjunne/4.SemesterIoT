/*
 * LoRaWANHandler.h
 *
 * Created: 21-04-2021 10:53:25
 *  Author: benja
 */ 

#pragma once
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>

MessageBufferHandle_t downlinkMessageBufferHandle;

void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

<<<<<<< Updated upstream:LoRaWANHandler.h
static void _lora_setup(void);

void lora_handler_task( void *pvParameters );
=======
void lora_handler_task( void *pvParameters );
void task_download( void *pvParameters );
>>>>>>> Stashed changes:networkdrivers/header/LoRaWANHandler.h

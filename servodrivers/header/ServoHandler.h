/*
 * ServoHandler.h
 *
 * Created: 27-05-2021 12:47:31
 *  Author: benja
 */ 
#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include "rc_servo.h"

void handleServoRequest(int requestcode);
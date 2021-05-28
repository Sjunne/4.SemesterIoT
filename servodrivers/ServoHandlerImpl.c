/*
 * ServoHandlerImpl.c
 *
 * Created: 27-05-2021 12:47:43
 *  Author: benja
 */ 

#include "../servodrivers/header/ServoHandler.h"
#include "../semaphore/header/testOutprint.h"


void handleServoRequest(int requestcode) {
	uint8_t servoNo = 1;
	int8_t percent;

	switch(requestcode) {
		case 1: {
			test_outprint("Water growbro \n");
			percent = 100;
			rc_servo_setPosition(servoNo, percent);
			break;
		}
		case 2: {
			test_outprint("Open window \n");
			percent = -100;
			rc_servo_setPosition(servoNo, percent);
			break;
		}
		case 3: {
			test_outprint("Close window \n");
			percent = -50;
			rc_servo_setPosition(servoNo, percent);
			break;
		}
		case 4: {
			test_outprint("Water growbro & open window \n");
			percent = 100;
			rc_servo_setPosition(servoNo, percent);
			percent = -100;
			rc_servo_setPosition(servoNo, percent);
			break;
		}
		case 5: {
			test_outprint("Water growbro & close window \n");
			percent = 100;
			rc_servo_setPosition(servoNo, percent);
			percent = -50;
			rc_servo_setPosition(servoNo, percent);
			break;
		}
		// If code doesn't represent something that we can use
		default: {
			puts("Unknown request code");
		}
	}
}
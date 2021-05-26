/*
* loraWANHandler.c
*
* Created: 12/04/2019 10:09:05
*  Author: IHA
*/
<<<<<<< Updated upstream:LoRaWANHandlerImpl.c
#include "LoRaWANHandler.h"
=======
#include "../networkdrivers/header/LoRaWANHandler.h"
#include "../data/header/SharedDataQueue.h"
#include "rc_servo.h"
>>>>>>> Stashed changes:networkdrivers/LoRaWANHandlerImpl.c

// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "BD80B543CA612714"
#define LORA_appKEY "8F4CE00051E8B6ADBC09BFDC65EED535"

<<<<<<< Updated upstream:LoRaWANHandlerImpl.c
static char _out_buf[100];
=======
//Lars' kode
//#define LORA_appEUI "BD80B543CA612714"
//#define LORA_appKEY "8F4CE00051E8B6ADBC09BFDC65EED535"
>>>>>>> Stashed changes:networkdrivers/LoRaWANHandlerImpl.c

void lora_handler_task( void *pvParameters );
void task_download( void *pvParameters );

static lora_driver_payload_t _uplink_payload;

void lora_handler_initialise(UBaseType_t lora_handler_task_priority)
{
	xTaskCreate(
	lora_handler_task
	,  "LRHand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
		xTaskCreate(
	task_download
	,  "LRHanddown"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

static void _lora_setup(void)
{
	lora_driver_returnCode_t rc;
	status_leds_slowBlink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;
	
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_longPuls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		printf("\t We have been accepted \n");
		status_leds_ledOn(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
	
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);

	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();

	printf("\t Do we get here? \n");

	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;

	TickType_t xLastWakeTime;
<<<<<<< Updated upstream:LoRaWANHandlerImpl.c
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
=======
<<<<<<< HEAD
	const TickType_t xFrequency = pdMS_TO_TICKS(400000UL); // UNDER TESTING 30000 (30 sec) Upload message every 5 minutes (300000 ms)
=======
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // UNDER TESTING 30000 (30 sec) Upload message every 5 minutes (300000 ms)
>>>>>>> 1413a59f4c1676bf811b30fc6b6b1aaefe980264
	xLastWakeTime = xTaskGetTickCount();
	uint16_t recieve;

	for(;;)
	{
		uint16_t growbroId = 1;
<<<<<<< HEAD
		
=======
					
			
>>>>>>> 1413a59f4c1676bf811b30fc6b6b1aaefe980264
		printf("\t going into delay \n");
>>>>>>> Stashed changes:networkdrivers/LoRaWANHandlerImpl.c
		xTaskDelayUntil( &xLastWakeTime, xFrequency );

		// Some dummy payload
		uint16_t hum = 12345; // Dummy humidity
		int16_t temp = 675; // Dummy temp
		uint16_t co2_ppm = 1050; // Dummy CO2
		// Make own payload consisting of struct

		_uplink_payload.bytes[0] = hum >> 8;
		_uplink_payload.bytes[1] = hum & 0xFF;
		_uplink_payload.bytes[2] = temp >> 8;
		_uplink_payload.bytes[3] = temp & 0xFF;
		_uplink_payload.bytes[4] = co2_ppm >> 8;
		_uplink_payload.bytes[5] = co2_ppm & 0xFF;

		status_leds_shortPuls(led_ST4);  // OPTIONAL
		printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
	
	}

}

void task_download( void *pvParameters )
{

	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);

	lora_driver_flushBuffers(); // get rid of first version string from module after reset!
	
	//// JUST FOR SERVO
	//rc_servo_initialise();
	//uint8_t servoNo = 1;
	//int8_t percent = 100;
	//rc_servo_setPosition(servoNo, percent);
	
	
	uint16_t recieve;
	for(;;)
	{	
		lora_driver_payload_t downlinkPayload;
		
		xMessageBufferReceive(downlinkMessageBufferHandle, &downlinkPayload, sizeof(lora_driver_payload_t), portMAX_DELAY);
		printf("DOWN LINK: from port: %d with %d bytes received!", downlinkPayload.portNo, downlinkPayload.len); // Just for Debug

		
		if (4 == downlinkPayload.len) // Check that we have got the expected 4 bytes
		{
			
		recieve = (downlinkPayload.bytes[0] << 8) + downlinkPayload.bytes[1];
		}

	}
}


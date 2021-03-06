/*
* loraWANHandler.c
*
* Created: 12/04/2019 10:09:05
*  Author: IHA
*/
#include "../networkdrivers/header/LoRaWANHandler.h"
#include "../data/header/SharedDataQueue.h"
#include "../semaphore/header/testOutprint.h"
#include "../servodrivers/header/ServoHandler.h"

// Parameters for OTAA join
#define LORA_appEUI "6818B76654F89545"
#define LORA_appKEY "B9AEC9CA7423D899CAA89AE8A165EBD0"

//Lars' kode
//#define LORA_appEUI "BD80B543CA612714"
//#define LORA_appKEY "8F4CE00051E8B6ADBC09BFDC65EED535"

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
	,  ((UBaseType_t)2)  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
		xTaskCreate(
	task_download
	,  "LRHanddown"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  ((UBaseType_t)3)  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

static void _lora_setup(void)
{
	char _out_buf[20];
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

	// DEFINING SIZE OF PAYLOAD
	_uplink_payload.len = 8;
	_uplink_payload.portNo = 2;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(600000UL); // Upload message every 10 minutes (600000 ms)
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		uint16_t growbroId = 1;
		
		puts("\t going into delay");
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		
		// Creating struct and dequeueing data
		SharedData_t sharedData;
		sharedData = dequeueSharedData();
		
		sprintf(printstring, "DEQUEUE: humidity: %d, co2: %d, Temp: %d \n", sharedData->humidity, sharedData->co2, sharedData->temperature);
		test_outprint(printstring);
		
		//printf("DEQUEUE: humidity: %d, co2: %d, Temp: %d \n", sharedData->humidity, sharedData->co2, sharedData->temperature);
		
		// CREATING OUR OWN PAYLOAD: GrowbroId, CO2, Humidity & Temperature
		_uplink_payload.bytes[0] = growbroId >> 8;
		_uplink_payload.bytes[1] = growbroId & 0xFF;
		_uplink_payload.bytes[2] = sharedData->co2 >> 8;
		_uplink_payload.bytes[3] = sharedData->co2 & 0xFF;
		_uplink_payload.bytes[4] = sharedData->humidity >> 8;
		_uplink_payload.bytes[5] = sharedData->humidity & 0xFF;
		_uplink_payload.bytes[6] = sharedData->temperature >> 8;
		_uplink_payload.bytes[7] = sharedData->temperature & 0xFF;
		
		// SENDING PAYLOAD	
		sprintf(printstring, "Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
		test_outprint(printstring);
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
	
	// Declaring requestCode & Payload
	uint16_t requestCode;
	lora_driver_payload_t downlinkPayload;
	
	// Setting portNo & length for payload
	downlinkPayload.portNo = 2;
	downlinkPayload.len = 4;

	for(;;)
	{		
		// Receiving from LoRaWAN 
		xMessageBufferReceive(downlinkMessageBufferHandle, &downlinkPayload, sizeof(lora_driver_payload_t), portMAX_DELAY);
		
		sprintf(printstring, "DOWN LINK: %d bytes received! \n", downlinkPayload.len);
		test_outprint(printstring);
		
		if (4 == downlinkPayload.len) // Check that we've received the expected 4 bytes
		{	
		//Converting bytes to integer
		requestCode = (downlinkPayload.bytes[2] << 8) + downlinkPayload.bytes[3];
		
		sprintf(printstring, "Requestcode received from downlink: %d \n", requestCode);
		test_outprint(printstring);
		
		//Calling servo to handle request
		handleServoRequest(requestCode);
		}
	}
}


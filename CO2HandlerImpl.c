/*
 * CO2HandlerImpl.c
 *
 * Created: 14-04-2021 14:05:30
 *  Author: Sjunn
 */ 

#include "CO2Handler.h"


void co2MeasureCallBack(uint16_t ppm)
{
	// Here we enqueue
	enqueueCO2Measure(ppm);
	
}

void co2Measure() {

	mh_z19_returnCode_t rc;
	
	rc = mh_z19_takeMeassuring();
	if (rc == MHZ19_OK)
	{
		mh_z19_injectCallBack(co2MeasureCallBack);
	}
	else
	{
		puts("det gik galt");
	}
}
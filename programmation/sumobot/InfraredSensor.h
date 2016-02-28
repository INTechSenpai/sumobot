#ifndef _INFRAREDSENSOR_h
#define _INFRAREDSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ProximitySensor.h"

class InfraredSensor : public ProximitySensor
{
public:
	InfraredSensor()
	{}

	uint32_t getMesure()
	{
		return 0;
	}
};

#endif


#ifndef _COLORSENSOR_h
#define _COLORSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ProximitySensor.h"

class ColorSensor : public ProximitySensor
{
public:
	ColorSensor()
	{}
	
	uint32_t getMesure()
	{
		return 0;
	}
};

#endif


#ifndef _TOFSENSOR_h
#define _TOFSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ProximitySensor.h"
#include "VL6180X.h"

class ToFSensor : public ProximitySensor
{
public:
	ToFSensor(uint8_t id, uint8_t pinStandby)
	{
		i2cAddress = id;
		this->pinStandby = pinStandby;
		standby();
		vlSensor.setTimeout(100);
	}

	uint32_t getMesure()
	{
		if (isON)
		{
			distance = vlSensor.readRangeSingle();
			if (vlSensor.timeoutOccurred())
			{
				distance = 0;
			}
		}
		else
		{
			distance = 0;
		}

		return distance;
	}

	void standby()
	{
		//vlSensor.stopContinuous();
		pinMode(pinStandby, OUTPUT);
		digitalWrite(pinStandby, LOW);
		isON = false;
	}

	void powerON()
	{
		pinMode(pinStandby, INPUT);
		vlSensor.init();
		vlSensor.configureDefault();
		vlSensor.setAddress(i2cAddress);
		//vlSensor.startRangeContinuous(100);
		isON = true;
	}

private:
	uint8_t i2cAddress, pinStandby;
	uint32_t distance;
	bool isON;
	VL6180X vlSensor;
};

#endif


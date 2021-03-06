#ifndef _INFRAREDSENSOR_h
#define _INFRAREDSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ProximitySensor.h"
#include "Wire\Wire.h"

class InfraredSensor : public ProximitySensor
{
public:
	InfraredSensor(uint8_t address)
	{
		this->address = address >> 1;
		distance = 0;
		shift = 0x02;
		distanceRegister = 0x5E;
	}

	void init()
	{
		// Lecture du "SHIFT bit" du capteur
		Wire.beginTransmission(address);
		Wire.write(0x35);	// L'adresse du SHIFT bit est 0x35
		Wire.endTransmission();

		Wire.requestFrom(address, (uint8_t)1);
		while (Wire.available() == 0) { ; }
		shift = Wire.read();
	}

	uint32_t getMesure()
	{
		Wire.beginTransmission(address);
		Wire.write(distanceRegister);
		Wire.endTransmission();

		Wire.requestFrom(address, (uint8_t)2);

		while (Wire.available() < 2) { ; }

		/* Lecture de la distance mesur�e par le capteur */
		high = Wire.read();
		low = Wire.read();

		/* Conversion en cm */
		distance = high << 4;
		distance += low;
		distance = distance >> (4 + shift);

		return distance*10; /* Conversion en mm */
	}

private:
	uint8_t address, distanceRegister;
	int distance;
	int low, high, shift;
};

#endif


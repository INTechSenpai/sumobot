#include "ColorSensor.h"

ColorSensor::ColorSensor()
{}

volatile uint32_t ColorSensor::endTimeAVg;
volatile uint32_t ColorSensor::endTimeAVd;
volatile uint32_t ColorSensor::endTimeARg;
volatile uint32_t ColorSensor::endTimeARd;
uint8_t ColorSensor::pinAVg;
uint8_t ColorSensor::pinAVd;
uint8_t ColorSensor::pinARg;
uint8_t ColorSensor::pinARd;
uint32_t ColorSensor::beginTime;
uint32_t ColorSensor::maxTime;
uint32_t ColorSensor::minTime;

void ColorSensor::init(uint8_t newPinAVg, uint8_t newPinAVd, uint8_t newPinARg, uint8_t newPinARd)
{
	pinAVg = newPinAVg;
	pinAVd = newPinAVd;
	pinARg = newPinARg;
	pinARd = newPinARd;

	beginTime = 0;
	endTimeAVg = 0;
	endTimeAVd = 0;
	endTimeARg = 0;
	endTimeARd = 0;
	
	attachInterrupt(pinAVg, av_g_interrupt, FALLING);
	attachInterrupt(pinAVd, av_d_interrupt, FALLING);
	attachInterrupt(pinARg, ar_g_interrupt, FALLING);
	attachInterrupt(pinARd, ar_d_interrupt, FALLING);

	minTime = 1;
	maxTime = 3000;
}

void ColorSensor::update()
{
	pinMode(pinAVg, OUTPUT);
	pinMode(pinAVd, OUTPUT);
	pinMode(pinARg, OUTPUT);
	pinMode(pinARd, OUTPUT);
	digitalWrite(pinAVg, HIGH);
	digitalWrite(pinAVd, HIGH);
	digitalWrite(pinARg, HIGH);
	digitalWrite(pinARd, HIGH);

	delayMicroseconds(10);

	pinMode(pinAVg, INPUT);
	pinMode(pinAVd, INPUT);
	pinMode(pinARg, INPUT);
	pinMode(pinARd, INPUT);
	digitalWrite(pinAVg, LOW);
	digitalWrite(pinAVd, LOW);
	digitalWrite(pinARg, LOW);
	digitalWrite(pinARd, LOW);

	beginTime = micros();
}

void ColorSensor::read(uint32_t & avG, uint32_t & avD, uint32_t & arG, uint32_t & arD, bool rawReading)
{
	static uint32_t endAVg, endAVd, endARg, endARd;
	cli();
	endAVg = endTimeAVg;
	endAVd = endTimeAVd;
	endARg = endTimeARg;
	endARd = endTimeARd;
	endTimeAVg = 0;
	endTimeAVd = 0;
	endTimeARg = 0;
	endTimeARd = 0;
	sei();

	if (beginTime < endAVg)
	{
		avG = endAVg - beginTime;
		if (!rawReading)
		{
			avG = CONSTRAIN(avG, minTime, maxTime);
			avG = map(avG, minTime, maxTime, 1, 1000);
		}
	}
	else
	{
		avG = 0;
	}

	if (beginTime < endAVd)
	{
		avD = endAVd - beginTime;
		if (!rawReading)
		{
			avD = CONSTRAIN(avD, minTime, maxTime);
			avD = map(avD, minTime, maxTime, 1, 1000);
		}
	}
	else
	{
		avD = 0;
	}

	if (beginTime < endARg)
	{
		arG = endARg - beginTime;
		if (!rawReading)
		{
			arG = CONSTRAIN(arG, minTime, maxTime);
			arG = map(arG, minTime, maxTime, 1, 1000);
		}
	}
	else
	{
		arG = 0;
	}

	if (beginTime < endARd)
	{
		arD = endARd - beginTime;
		if (!rawReading)
		{
			arD = CONSTRAIN(arD, minTime, maxTime);
			arD = map(arD, minTime, maxTime, 1, 1000);
		}
	}
	else
	{
		arD = 0;
	}
}

void ColorSensor::calibrateBlack()
{
	uint32_t newMaxTime = 0;
	uint32_t avG, avD, arG, arD;

	for (int i = 0; i < 50; i++)
	{
		update();
		delay(10);
		read(avG, avD, arG, arD, true);
		if (avG > newMaxTime)
			newMaxTime = avG;
		if (avD > newMaxTime)
			newMaxTime = avD;
		if (arG > newMaxTime)
			newMaxTime = arG;
		if (arD > newMaxTime)
			newMaxTime = arD;
	}
	maxTime = newMaxTime;
}

void ColorSensor::calibrateWhite()
{
	uint32_t newMinTime = UINT32_MAX;
	uint32_t avG, avD, arG, arD;

	for (int i = 0; i < 50; i++)
	{
		update();
		delay(10);
		read(avG, avD, arG, arD, true);
		if (avG < newMinTime)
			newMinTime = avG;
		if (avD < newMinTime)
			newMinTime = avD;
		if (arG < newMinTime)
			newMinTime = arG;
		if (arD < newMinTime)
			newMinTime = arD;
	}
	minTime = newMinTime;
}

void ColorSensor::av_g_interrupt()
{
	endTimeAVg = micros();
}

void ColorSensor::av_d_interrupt()
{
	endTimeAVd = micros();
}

void ColorSensor::ar_g_interrupt()
{
	endTimeARg = micros();
}

void ColorSensor::ar_d_interrupt()
{
	endTimeARd = micros();
}

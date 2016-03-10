#include "ColorSensor.h"

ColorSensor::ColorSensor()
{}

volatile uint32_t ColorSensor::endTimeAVg;
volatile uint32_t ColorSensor::endTimeAVd;
volatile uint32_t ColorSensor::endTimeARg;
volatile uint32_t ColorSensor::endTimeARd;

uint8_t 
ColorSensor::powerAVg, ColorSensor::intAVg, 
ColorSensor::powerAVd, ColorSensor::intAVd, 
ColorSensor::powerARg, ColorSensor::intARg, 
ColorSensor::powerARd, ColorSensor::intARd;

uint32_t ColorSensor::beginTime;
uint32_t ColorSensor::maxTime;
uint32_t ColorSensor::minTime;

void ColorSensor::init(uint8_t pAVg, uint8_t iAVg, uint8_t pAVd, uint8_t iAVd, uint8_t pARg, uint8_t iARg, uint8_t pARd, uint8_t iARd)
{
	powerAVg = pAVg;
	powerAVd = pAVd;
	powerARg = pARg;
	powerARd = pARd;
	
	intAVg = iAVg;
	intAVd = iAVd;
	intARg = iARg;
	intARd = iARd;

	beginTime = 0;
	endTimeAVg = 0;
	endTimeAVd = 0;
	endTimeARg = 0;
	endTimeARd = 0;

	pinMode(intAVg, INPUT_PULLUP);
	pinMode(intAVd, INPUT_PULLUP);
	pinMode(intARg, INPUT_PULLUP);
	pinMode(intARd, INPUT_PULLUP);
	
	attachInterrupt(intAVg, av_g_interrupt, FALLING);
	attachInterrupt(intAVd, av_d_interrupt, FALLING);
	attachInterrupt(intARg, ar_g_interrupt, FALLING);
	attachInterrupt(intARd, ar_d_interrupt, FALLING);

	minTime = 1;
	maxTime = 8000;
}

void ColorSensor::update()
{
	pinMode(powerAVg, OUTPUT);
	pinMode(powerAVd, OUTPUT);
	pinMode(powerARg, OUTPUT);
	pinMode(powerARd, OUTPUT);
	digitalWrite(powerAVg, HIGH);
	digitalWrite(powerAVd, HIGH);
	digitalWrite(powerARg, HIGH);
	digitalWrite(powerARd, HIGH);

	delayMicroseconds(10);

	pinMode(powerAVg, INPUT);
	pinMode(powerAVd, INPUT);
	pinMode(powerARg, INPUT);
	pinMode(powerARd, INPUT);
	digitalWrite(powerAVg, LOW);
	digitalWrite(powerAVd, LOW);
	digitalWrite(powerARg, LOW);
	digitalWrite(powerARd, LOW);

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

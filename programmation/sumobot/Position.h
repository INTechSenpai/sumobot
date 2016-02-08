#ifndef _POSITION_h
#define _POSITION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct Position
{
	int16_t x;
	int16_t y;
	float orientation;
	float xSpeed;
	float ySpeed;
};

struct PositionUncertainty
{
	int16_t x;
	int16_t y;
	float orientation;
	float xSpeed;
	float ySpeed;
};

#endif
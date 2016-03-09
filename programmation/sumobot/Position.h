#ifndef _POSITION_h
#define _POSITION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
//#include "WProgram.h"
#endif

struct Position
{
	float x;
	float y;
	float orientation;
	float xSpeed;
	float ySpeed;
};

struct PositionUncertainty
{
	float x;
	float y;
	float orientation;
	float xSpeed;
	float ySpeed;
};

#endif

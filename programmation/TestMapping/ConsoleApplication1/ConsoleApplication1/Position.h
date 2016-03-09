#ifndef _POSITION_h
#define _POSITION_h

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
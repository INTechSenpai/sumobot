#ifndef _OBSTACLE_h
#define _OBSTACLE_h
#endif

#include "Position.h"
class Obstacle
{
public:
	Obstacle();
	Obstacle(float x, float y, float orientation, float xSpeed, float ySpeed);
	Position getPosition();
	~Obstacle();

private:
	Position position;
};


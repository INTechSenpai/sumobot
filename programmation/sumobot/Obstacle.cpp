#include "Obstacle.h"


Obstacle::Obstacle()
{
}

Obstacle::Obstacle(float x, float y, float orientation, float xSpeed, float ySpeed)
{
	position.x = x;
	position.y = y;
	position.orientation = orientation;
	position.xSpeed = xSpeed;
	position.ySpeed = ySpeed;
}


Obstacle::~Obstacle()
{
}

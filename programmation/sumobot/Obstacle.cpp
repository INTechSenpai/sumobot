#include "Obstacle.h"
#define _USE_MATH_DEFINES
#include "math.h"

Obstacle::Obstacle()
{
}

Obstacle::Obstacle(float x, float y, float orientation, float xSpeed, float ySpeed) //position x,y orientation, vitesse en x,y
{
	this->position.x = x;
	this->position.y = y;
	this->position.orientation = orientation;
	this->position.xSpeed = xSpeed;
	this->position.ySpeed = ySpeed;
}


Obstacle::~Obstacle()
{
}

Position Obstacle::getPosition()
{
	return this->position;
}

void Obstacle::setPosition(Position p)
{
	this->position = p;
	return;
}

/*
Calcul de angles avec arctan et tan=Opposé/adjacent
beaucoup de possibilité en fonction des signes de xSpeed et ySpeed
*/

float Obstacle::calculOrientation()
{
	if (/* this->position.ySpeed >= 0 && */this->position.xSpeed < 0)
	{
		return this->position.orientation = M_PI + atan(this->position.ySpeed / this->position.xSpeed);
	}

	if (this->position.ySpeed > 0 && this->position.xSpeed > 0)
	{
		return this->position.orientation = atan(this->position.ySpeed / this->position.xSpeed);
	}
		
	if (this->position.ySpeed < 0 && this->position.xSpeed > 0)
	{
		return this->position.orientation = 2 * M_PI + atan(this->position.ySpeed / this->position.xSpeed);
	}

	/*if (this->position.ySpeed < 0 && this->position.xSpeed < 0)
	{
		return this->position.orientation = M_PI + atan(this->position.ySpeed / this->position.xSpeed);
	}
	Ce if reviens au même que celui du debut, d ou la simplification
	*/

	if (this->position.ySpeed == 0 && this->position.xSpeed == 0)
	{
		return this->position.orientation;
	}

	if (this->position.ySpeed == 0 && this->position.xSpeed < 0)
	{
		return this->position.orientation = M_PI;
	}

	if (this->position.ySpeed == 0 && this->position.xSpeed > 0)
	{
		return this->position.orientation = 0;
	}

	if (this->position.ySpeed < 0 && this->position.xSpeed == 0)
	{
		return this->position.orientation = 3 * M_PI / 2;
	}

	if (this->position.ySpeed > 0 && this->position.xSpeed == 0)
	{
		return this->position.orientation = M_PI / 2;
	}

}
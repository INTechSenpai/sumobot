#include "ObstacleCercle.h"


ObstacleCercle::ObstacleCercle()
{
}

ObstacleCercle::ObstacleCercle(float x, float y, float orientation, float xSpeed, float ySpeed, float r)
{
	Obstacle(x, y, orientation, xSpeed, ySpeed);
	this->rayon = r;
}

ObstacleCercle::~ObstacleCercle()
{
}

float ObstacleCercle::getRayon()
{
	return this->rayon;
}

void ObstacleCercle::setRayon(float r)
{
	this->rayon = r;
	return;
}
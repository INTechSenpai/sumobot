#include "ObstacleCercle.h"


ObstacleCercle::ObstacleCercle()
{
}

ObstacleCercle::ObstacleCercle(float x, float y, float orientation, float xSpeed, float ySpeed, float r)
{
	Obstacle(x, y, orientation, xSpeed, ySpeed);
	rayon = r;
}

ObstacleCercle::~ObstacleCercle()
{
}

float ObstacleCercle::getRayon()
{
	return rayon;
}
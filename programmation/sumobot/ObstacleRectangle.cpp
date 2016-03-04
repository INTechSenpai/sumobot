#include "ObstacleRectangle.h"


ObstacleRectangle::ObstacleRectangle()
{
}

ObstacleRectangle::ObstacleRectangle(float x, float y, float orientation, float xSpeed, float ySpeed, float lon, float larg)
{
	Obstacle(x, y, orientation, xSpeed, ySpeed);
	largeur = larg;
	longueur = lon;
}

ObstacleRectangle::~ObstacleRectangle()
{
}

float ObstacleRectangle::getLargeur()
{
	return largeur;
}

float ObstacleRectangle::getLongueur()
{
	return longueur;
}
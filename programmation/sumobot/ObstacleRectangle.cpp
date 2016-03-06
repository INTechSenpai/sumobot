#include "ObstacleRectangle.h"


ObstacleRectangle::ObstacleRectangle()
{
}

ObstacleRectangle::ObstacleRectangle(float x, float y, float orientation, float xSpeed, float ySpeed, float lon, float larg)
{
	Obstacle(x, y, orientation, xSpeed, ySpeed);
	this->largeur = larg;
	this->longueur = lon;
}

ObstacleRectangle::~ObstacleRectangle()
{
}

float ObstacleRectangle::getLargeur()
{
	return this->largeur;
}

float ObstacleRectangle::getLongueur()
{
	return this->longueur;
}

void ObstacleRectangle::setLongueur(float lon)
{
	this->longueur = lon;
	return;
}

void ObstacleRectangle::setLargeur(float larg)
{
	this->largeur = larg;
	return;
}

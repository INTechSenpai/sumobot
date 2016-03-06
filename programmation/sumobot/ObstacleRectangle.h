#pragma once

#ifndef _OBSTACLERECTANGLE_h
#define _OBSTACLERECTANGLE_h
#endif

#include "Obstacle.h"

class ObstacleRectangle :
	public Obstacle
{
public:

	ObstacleRectangle();
	ObstacleRectangle(float x, float y, float orientation, float xspeed, float yspeed, float longueur, float largeur);
	~ObstacleRectangle();
	float getLongueur();
	float getLargeur();
	void setLongueur(float lon);
	void setLargeur(float larg);

private:

	float longueur;
	float largeur;

};


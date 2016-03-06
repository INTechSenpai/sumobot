#pragma once
#ifndef _OBSTACLECERCLE_h
#define _OBSTACLECERCLE_h
#endif

#include "Obstacle.h"

class ObstacleCercle :
	public Obstacle
{
public:

	ObstacleCercle();
	ObstacleCercle(float x, float y, float orientation, float xSpeed, float ySpeed, float r); 
	~ObstacleCercle();
	float getRayon();
	float setRayon();
	void setRayon(float r);

private:

	float rayon;

};


#ifndef _POSITION_h
#define _POSITION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/*
	Cette classe définit la position d'un solide sur la table.
	Le solide est repéré par un point (x,y), une orientation, ainsi qu'un vecteur
	représentant sa vitesse instantanée (Vx,Vy).
	La vitesse de rotation n'est pas incluse, elle a été jugée inutile.

	La position (x,y,orientation) est entachée d'une certaine incertitude.
	Le vrai 'x' est compris entre 'x + xUncertainty' et 'x - xUncertainty'
	(idem pour les deux autres variables y et orientation)

	Unités : 
		mm pour les longueurs
		mm/s pour les vitesses
		radians pour les angles
*/



class Position
{
public:

	Position()
	{
		x = 0;
		y = 0;
		orientation = 0;
		xSpeed = 0;
		ySpeed = 0;
		xUncertainty = 0;
		yUncertainty = 0;
		oUncertainty = 0;
	}

	Position(float x, float y, float orientation)
	{
		this->x = x;
		this->y = y;
		this->orientation = orientation;
		xSpeed = 0;
		ySpeed = 0;
		xUncertainty = 0;
		yUncertainty = 0;
		oUncertainty = 0;
	}

	float x;
	float y;
	float orientation;
	float xSpeed;
	float ySpeed;
	float xUncertainty;
	float yUncertainty;
	float oUncertainty;
};

#endif

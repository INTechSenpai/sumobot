#ifndef _POSITION_h
#define _POSITION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/*
	Cette classe d�finit la position d'un solide sur la table.
	Le solide est rep�r� par un point (x,y), une orientation, ainsi qu'un vecteur
	repr�sentant sa vitesse instantan�e (Vx,Vy).
	La vitesse de rotation n'est pas incluse, elle a �t� jug�e inutile.

	La position (x,y,orientation) est entach�e d'une certaine incertitude.
	Le vrai 'x' est compris entre 'x + xUncertainty' et 'x - xUncertainty'
	(idem pour les deux autres variables y et orientation)

	Unit�s : 
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
	}

	Position(float x, float y, float orientation)
	{
		this->x = x;
		this->y = y;
		this->orientation = orientation;
	}

	void operator= (volatile const Position & newPosition) volatile
	{
		this->x = newPosition.x;
		this->y = newPosition.y;
		this->orientation = newPosition.orientation;
	}

	float x;
	float y;
	float orientation;
};

#endif

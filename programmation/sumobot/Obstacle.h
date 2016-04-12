#ifndef _OBSTACLE_h
#define _OBSTACLE_h

#include "Position.h"
#include "stdint.h"
#include <vector>


/* Valeurs sp�ciales de OrientationToPush */
#define ORIENTATION_UNDEFINED	-1	// Orientation indiff�rente
#define ORIENTATION_FORWARD		-2	// Orientation indiff�rente mais il faut pousser en marche avant
#define ORIENTATION_BACKWARD	-3	// Orientation indiff�rente mais il faut pousser en marche arri�re


enum ObstacleShape
{
	CIRCLE, RECTANGLE, EDGE_OF_TABLE
};


/* Cette classe d�fini les "Obstacles" que le robot devra contourner, pousser, detecter, identifier... */
class Obstacle
{
public:
	Obstacle(Position & center, ObstacleShape circleOrRectangle)
	{
		this->center = center;
		type = circleOrRectangle;

		radius = 0;
		xRadius = 0;
		yRadius = 0;
		toPush = false;
		lastTimeSeen = 0;
		timeToLive = UINT32_MAX;
		priority = 0;
		orientationToPush = ORIENTATION_UNDEFINED;
	}


	/* Indique de d�sormai l'obstacle devra �tre pouss� et non plus �vit� */
	void pushIt()
	{
		if (!anObstacleIsPushed) // Un seul obstacle peut �tre pouss� � la fois, on v�rifie donc qu'aucun autre obstacle est pouss�.
		{
			toPush = true;
			anObstacleIsPushed = true;
		}
	}

	/* Indique que l'obstacle doit de nouveau �tre �vit� */
	void stopPushing()
	{
		if (toPush)
		{
			toPush = false;
			anObstacleIsPushed = false;
		}
	}




	/* GETTERS & SETTERS */

	bool isPushed()
	{
		return toPush;
	}

	static bool isAnObstaclePushed()
	{
		return anObstacleIsPushed;
	}

	void justSeen() // 'setter' de l'attibut lastTimeSeen
	{
		lastTimeSeen = millis();
	}

	uint32_t getLastTimeSeen()
	{
		return lastTimeSeen;
	}

	void getCenter(Position & ici) const
	{
		ici = center;
	}

	void setCenter(const Position & ici)
	{
		center = ici;
	}

	ObstacleShape getShape() const
	{
		return type;
	}

	void setRadius(float newRadius)
	{
		if (type == CIRCLE)
			radius = newRadius;
	}

	float getRadius() const
	{
		return radius;
	}

	void setXRadius(float newX)
	{
		if (type == RECTANGLE || type == EDGE_OF_TABLE)
			xRadius = newX;
	}

	float getXRadius() const
	{
		return xRadius;
	}

	void setYRadius(float newY)
	{
		if (type == RECTANGLE || type == EDGE_OF_TABLE)
			yRadius = newY;
	}

	float getYRadius() const
	{
		return yRadius;
	}

	void setPriority(uint8_t priority)
	{
		this->priority = priority;
	}

	uint8_t getPriority()
	{
		return priority;
	}

	void setOrientationToPush(float orientation)
	{
		orientationToPush = orientation;
	}

	float getOrientationToPush()
	{
		return orientationToPush;
	}

private:
	Position center; // Point autour duquel l'obstacle est centr�
	uint32_t lastTimeSeen; // Date � laquelle l'obstacle a �t� d�tect� pour la derni�re fois (en ms)
	uint32_t timeToLive;  // Dur�e de vie de l'obstacle lorsqu'il est hors de vue (en ms)

	bool toPush; // Indique si cet obstacle doit �tre pouss� au lieu d'�tre �vit�. Un seul obstacle peut �tre pouss� � la fois
	static bool anObstacleIsPushed;
	float orientationToPush;	// D�signe l'orientation que le robot doit avoir au moment o� il entre en contact avec l'obstacle pour le pousser. La valeur est comprise entre 0 et 2pi

	ObstacleShape type; // Indique si la forme de l'obstacle est un CERCLE, un RECTANGLE, ou bien un EDGE_OF_TABLE (un rectangle dont la zone interdite est l'ext�rieur)
	uint8_t priority;	// Correspond � une quantit� de points rapport�s par l'objet s'il est rapport� par le robot. Par d�faut : 0

	/* Dans le cas o� l'obstacle est un cercle, ce champ est renseign� ( !=0 ) */
	float radius; // Rayon de l'obstacle (en mm)

	/* Dans le cas o� l'obstacle est un rectangle, ces trois champs sont renseign�s ( x ou y est !=0 ) */
	float xRadius;
	float yRadius;
};


/* Contient l'ensemble des obstacles de la table (connus par le robot) */
class ObstacleMap
{
public:
	ObstacleMap()
	{}
	std::vector<Obstacle> fixedVisible;		// Obstacles fixes au cours du match et d�tectables par les capteurs
	std::vector<Obstacle> fixedInvisible;	// Obstacles fixes au cours du match et invisibles pour les capteurs
	std::vector<Obstacle> oponentRobot;		// Obstacles repr�sentant les robots adverses (� contourner)
	std::vector<Obstacle> movableVisible;	// Obstacles d�pla�ables par le robot et d�tectables
	std::vector<Obstacle> movableInvisible;	// Obstacles d�pla�ables par le robot mais invisibles
	std::vector<Obstacle> toBeSpecified;	// Obstacles ayant �t� vus par les capteurs mais pas encore interpr�t�s
};



#endif

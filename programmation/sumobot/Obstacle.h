#ifndef _OBSTACLE_h
#define _OBSTACLE_h

#include "Position.h"
#include "stdint.h"
#include <vector>

enum ObstacleShape
{
	CIRCLE, RECTANGLE, EDGE_OF_TABLE
};


/* Cette classe défini les "Obstacles" que le robot devra contourner, pousser, detecter, identifier... */
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
		priority = 0;
		orientationToPush = -1;
	}


	/* Indique de désormai l'obstacle devra être poussé et non plus évité */
	void pushIt()
	{
		if (!anObstacleIsPushed) // Un seul obstacle peut être poussé à la fois, on vérifie donc qu'aucun autre obstacle est poussé.
		{
			toPush = true;
			anObstacleIsPushed = true;
		}
	}

	/* Indique que l'obstacle doit de nouveau être évité */
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
	Position center; // Point autour duquel l'obstacle est centré
	uint32_t lastTimeSeen; // Date à laquelle l'obstacle a été détecté pour la dernière fois (en ms)

	bool toPush; // Indique si cet obstacle doit être poussé au lieu d'être évité. Un seul obstacle peut être poussé à la fois
	static bool anObstacleIsPushed;
	float orientationToPush;	// Désigne l'orientation que le robot doit avoir au moment où il entre en contact avec l'obstacle pour le pousser. La valeur '-1' signifie 'indifférent'

	ObstacleShape type; // Indique si la forme de l'obstacle est un CERCLE, un RECTANGLE, ou bien un EDGE_OF_TABLE (un rectangle dont la zone interdite est l'extérieur)
	uint8_t priority;	// Correspond à une quantité de points rapportés par l'objet s'il est rapporté par le robot. Par défaut : 0

	/* Dans le cas où l'obstacle est un cercle, ce champ est renseigné ( !=0 ) */
	float radius; // Rayon de l'obstacle (en mm)

	/* Dans le cas où l'obstacle est un rectangle, ces trois champs sont renseignés ( x ou y est !=0 ) */
	float xRadius;
	float yRadius;
};


/* Contient l'ensemble des obstacles de la table (connus par le robot) */
class ObstacleMap
{
public:
	ObstacleMap()
	{}
	std::vector<Obstacle> fixedVisible;		// Obstacles fixes au cours du match et détectables par les capteurs
	std::vector<Obstacle> fixedInvisible;	// Obstacles fixes au cours du match et invisibles pour les capteurs
	std::vector<Obstacle> oponentRobot;		// Obstacles représentant les robots adverses (à contourner)
	std::vector<Obstacle> movableVisible;	// Obstacles déplaçables par le robot et détectables
	std::vector<Obstacle> movableInvisible;	// Obstacles déplaçables par le robot mais invisibles
	std::vector<Obstacle> toBeSpecified;	// Obstacles ayant été vus par les capteurs mais pas encore interprétés
};



#endif

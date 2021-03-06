#ifndef _PATH_h
#define _PATH_h

/*
	D�finition les types Trajectory et UnitMove
	Ces types repr�sentent respectivement une trajectoire et un mouvement �l�mentaire.
	Une trajectoire est une succession de mouvements �l�mentaires enchain�s (sans arr�t du robot entre chaque mouvement)
	Il est d�conseill� de tenter de parcourir des trajectoires trop discontinues. En cas de variations brutales de 
	direction il est recommand� de d�composer le mouvement en plusieurs trajectoires.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <vector>

/* Conversion Tick-mm et Radians-mm */
#define TICK_TO_MM 0.085			// unit� : mm/ticks
#define TICK_TO_RADIAN 0.001406		// unit� : radians/ticks

#define COS_PI_4	0.7071067811865	// cos(PI/4) utilis� pour les calculs


/* Le rayon "infini" permet de d�finir une trajectoire rectiligne
	(l'unit� de cette constante est le Ticks)
 */
#define INFINITE_RADIUS 42424

/* Un mouvement �l�mentaire est repr�sent� par un rayon de courbure et une longueur
	Toutes les unit�s sont en ticks (unit� de longueur ou d'angle selon le contexte).

	### CAS PARTICULIER ###
	Si le rayon de courbure est nul
	La trajectoire est alors une rotation pure
	La "longueur" repr�sente alors l'angle de rotation
	Un angle positif correspond � une rotation trigonom�trique,
	� l'inverse : n�gatif = antitrigo.
	###					###

	Concernant les sens par rapport aux signes :
		Un rayon de courbure positif repr�sente un virage � gauche
		Un rayon de courbure n�gatif repr�sente un virage � droite
		Une longueur positive repr�sente une marche avant
		Une longueur n�gative repr�sente une marche arri�re

		En r�sum� :
				   Avant
				  \	 +  /
		           \   / 
		            \_/
		Gauche	+	|R|	  -		Droite
				    / \
				   /   \
				  /  -  \
				  Arri�re

*/
class UnitMove
{
public:
	UnitMove()
	{
		length = 0;
		bendRadius = INFINITE_RADIUS;
		speed = 350;
		stopAfterMove = true;
	}

	bool stopAfterMove; // Indique si le robot doit s'arr�ter � la fin de ce mouvement avant d'enchainer sur le suivant

	void setLengthMm(int32_t lengthMM)
	{
		length = lengthMM / TICK_TO_MM;
	}
	void setLengthTicks(int32_t lengthTicks)
	{
		length = lengthTicks;
	}
	void setLengthRadians(float lengthRadians)
	{
		length = lengthRadians / TICK_TO_RADIAN;
	}
	int32_t getLengthMm() const
	{
		return length * TICK_TO_MM;
	}
	int32_t getLengthTicks() const
	{
		return length;
	}
	float getLengthRadians() const
	{
		return length * TICK_TO_RADIAN;
	}

	void setBendRadiusMm(int32_t bendRadiusMM)
	{
		if (bendRadiusMM == INFINITE_RADIUS)
		{
			bendRadius = INFINITE_RADIUS;
		}
		else
		{
			bendRadius = bendRadiusMM;
			bendRadius *= COS_PI_4;
			bendRadius /= TICK_TO_MM;
		}
	}
	void setBendRadiusTicks(int32_t bendRadiusTicks)
	{
		bendRadius = bendRadiusTicks;
	}
	int32_t getBendRadiusMm() const
	{
		return bendRadius * TICK_TO_MM;
	}
	int32_t getBendRadiusTicks() const
	{
		return bendRadius;
	}

	void setSpeedMm_S(int32_t speedMM_S)
	{
		speed = speedMM_S / TICK_TO_MM;
	}
	void setSpeedTicks_S(int32_t speedTicks_S)
	{
		speed = speedTicks_S;
	}
	int32_t getSpeedMm_S() const
	{
		return speed * TICK_TO_MM;
	}
	int32_t getSpeedTicks_S() const
	{
		return speed;
	}

private:
	int32_t length; // Longueur du mouvement, en ticks
	int32_t bendRadius; // Rayon de courbure, en ticks
	int32_t speed;	// Vitesse de d�placement (POSITIVE), en ticks/s
};


/* Une trajectoire est repr�sent�e par un tableau de mouvements �l�mentaires
Le premier mouvement effectu� est celui d'indice 0 dans ce tableau */
typedef std::vector<UnitMove> Trajectory;

#endif
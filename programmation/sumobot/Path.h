#ifndef _PATH_h
#define _PATH_h

/*
	Définition les types Trajectory et UnitMove
	Ces types représentent respectivement une trajectoire et un mouvement élémentaire.
	Une trajectoire est une succession de mouvements élémentaires enchainés (sans arrêt du robot entre chaque mouvement)
	Il est déconseillé de tenter de parcourir des trajectoires trop discontinues. En cas de variations brutales de 
	direction il est recommandé de décomposer le mouvement en plusieurs trajectoires.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <vector>

/* Conversion Tick-mm et Radians-mm */
#define TICK_TO_MM 0.085			// unité : mm/ticks
#define TICK_TO_RADIAN 0.001406		// unité : radians/ticks

#define COS_PI_4	0.7071067811865	// cos(PI/4) utilisé pour les calculs


/* Le rayon "infini" permet de définir une trajectoire rectiligne
	(l'unité de cette constante est le Ticks, mais du coup à fortiori ça marchera si c'est des mm ^^)
 */
#define INFINITE_RADIUS 424242424

/* Un mouvement élémentaire est représenté par un rayon de courbure et une longueur
	Toutes les unités sont en ticks (unité de longueur ou d'angle selon le contexte).

	### CAS PARTICULIER ###
	Si le rayon de courbure est nul
	La trajectoire est alors une rotation pure
	La "longueur" représente alors l'angle de rotation
	Un angle positif correspond à une rotation trigonométrique,
	à l'inverse : négatif = antitrigo.
	###					###

	Concernant les sens par rapport aux signes :
		Un rayon de courbure positif représente un virage à gauche
		Un rayon de courbure négatif représente un virage à droite
		Une longueur positive représente une marche avant
		Une longueur négative représente une marche arrière

		En résumé :
				   Avant
				  \	 +  /
		           \   / 
		            \_/
		Gauche	+	|R|	  -		Droite
				    / \
				   /   \
				  /  -  \
				  Arrière

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

	bool stopAfterMove; // Indique si le robot doit s'arrêter à la fin de ce mouvement avant d'enchainer sur le suivant

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
	int32_t getLengthMm()
	{
		return length * TICK_TO_MM;
	}
	int32_t getLengthTicks()
	{
		return length;
	}
	float getLengthRadians()
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
	int32_t getBendRadiusMm()
	{
		return bendRadius * TICK_TO_MM;
	}
	int32_t getBendRadiusTicks()
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
	int32_t getSpeedMm_S()
	{
		return speed * TICK_TO_MM;
	}
	int32_t getSpeedTicks_S()
	{
		return speed;
	}

private:
	int32_t length; // Longueur du mouvement, en ticks
	int32_t bendRadius; // Rayon de courbure, en ticks
	int32_t speed;	// Vitesse de déplacement (POSITIVE), en ticks/s
};


/* Une trajectoire est représentée par un tableau de mouvements élémentaires
Le premier mouvement effectué est celui d'indice 0 dans ce tableau */
typedef std::vector<UnitMove> Trajectory;

#endif
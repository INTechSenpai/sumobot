#ifndef _RELATIVEOBSTACLEMAP_h
#define _RELATIVEOBSTACLEMAP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


/* 
	Représente d'état des capteurs du robot 
	Une valeur à 0 = non significatif (erreur de lecture, capteur non branché ou bien valeur déjà lue)

	Pour les capteurs de proximité (tous sauf ceux du sol) l'unité est le mm.
	Pour les capteurs noir/blanc (ceux du sol) "1" correspond au blanc parfait et "1000" au noir parfait. Les valeurs intermédiaires sont des nuances de gris.

	Les vitesses sont exprimées en mm/s (il s'agit de la vitesse de variation des valeurs mesurées par chaque capteur)
*/

struct RelativeObstacleMap
{
	uint32_t avantGauche;
	float speedAvantGauche;
	uint32_t avantDroit;
	float speedAvantDroit;
	uint32_t avant;
	float speedAvant;
	uint32_t gauche;
	float speedGauche;
	uint32_t droit;
	float speedDroit;
	uint32_t arriere;
	float speedArriere;
	uint32_t arriereGauche;
	float speedArriereGauche;
	uint32_t arriereDroit;
	float speedArriereDroit;
	uint32_t solAvantGauche;
	uint32_t solAvantDroit;
	uint32_t solArriereGauche;
	uint32_t solArriereDroit;
};

#endif


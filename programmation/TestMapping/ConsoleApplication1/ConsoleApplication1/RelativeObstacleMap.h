#ifndef _RELATIVEOBSTACLEMAP_h
#define _RELATIVEOBSTACLEMAP_h

#include "stdint.h"
/* 
	Représente d'état des capteurs du robot 
	Une valeur à 0 = non significatif (erreur de lecture ou bien valeur déjà lue)
*/

struct RelativeObstacleMap
{
	float speedAvantGauche;
	float speedAvantDroit;
	float speedAvant;
	float speedGauche;
	float speedDroit;
	float speedArriere;
	float speedArriereGauche;
	float speedArriereDroit;
	uint32_t avantGauche;
	uint32_t avantDroit;
	uint32_t avant;
	uint32_t gauche;
	uint32_t droit;
	uint32_t arriere;
	uint32_t arriereGauche;
	uint32_t arriereDroit;
	uint32_t solAvantGauche;
	uint32_t solAvantDroit;
	uint32_t solArriereGauche;
	uint32_t solArriereDroit;
};

#endif


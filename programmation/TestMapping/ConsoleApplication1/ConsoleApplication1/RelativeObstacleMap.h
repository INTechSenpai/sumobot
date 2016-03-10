#ifndef _RELATIVEOBSTACLEMAP_h
#define _RELATIVEOBSTACLEMAP_h

#include "stdint.h"
/* 
	Représente d'état des capteurs du robot 
	Une valeur à 0 = non significatif (erreur de lecture ou bien valeur déjà lue)
	capteur infrarouge infini=630
	capteur infrarouge tof infini=255
*/

struct RelativeObstacleMap
{
	uint32_t avantGauche;
	int32_t speedAvantGauche;
	uint32_t avantDroit;
	int32_t speedAvantDroit;
	uint32_t avant;
	int32_t speedAvant;
	uint32_t gauche;
	int32_t speedGauche;
	uint32_t droit;
	int32_t speedDroit;
	uint32_t arriere;
	int32_t speedArriere;
	uint32_t arriereGauche;
	int32_t speedArriereGauche;
	uint32_t arriereDroit;
	int32_t speedArriereDroit;
	uint32_t solAvantGauche;
	uint32_t solAvantDroit;
	uint32_t solArriereGauche;
	uint32_t solArriereDroit;
};

#endif


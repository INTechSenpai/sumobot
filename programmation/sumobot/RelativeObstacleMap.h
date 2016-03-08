#ifndef _RELATIVEOBSTACLEMAP_h
#define _RELATIVEOBSTACLEMAP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


/* 
	Repr�sente d'�tat des capteurs du robot 
	Une valeur � 0 = non significatif (erreur de lecture, capteur non branch� ou bien valeur d�j� lue)
	Dans ce cas la valeur de vitesse associ�e n'est pas non plus significative

	Pour les capteurs de proximit� (tous sauf ceux du sol) l'unit� est le mm.
	Pour les capteurs noir/blanc (ceux du sol) "1" correspond au blanc parfait et "1000" au noir parfait. Les valeurs interm�diaires sont des nuances de gris.

	Les vitesses sont exprim�es en mm/s (il s'agit de la vitesse de variation des valeurs mesur�es par chaque capteur)
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


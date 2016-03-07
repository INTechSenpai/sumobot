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

	Pour les capteurs de proximit� (tous sauf ceux du sol) l'unit� est le mm.
	Pour les capteurs noir/blanc (ceux du sol) "1" correspond au blanc parfait et "1000" au noir parfait. Les valeurs interm�diaires sont des nuances de gris.
*/

struct RelativeObstacleMap
{
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


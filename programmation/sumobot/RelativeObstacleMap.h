#ifndef _RELATIVEOBSTACLEMAP_h
#define _RELATIVEOBSTACLEMAP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define IR_INFINITY		630
#define TOF_INFINITY	255

/* 
	Représente d'état des capteurs du robot 
	Une valeur à 0 = non significatif (erreur de lecture, capteur non branché ou bien valeur déjà lue)
	Dans ce cas la valeur de vitesse associée n'est pas non plus significative

	Si la valeur lue est égale à "INFINITY" le capteur ne détecte rien. La valeur de vitesse associée n'est pas significative.

	Pour les capteurs de proximité (tous sauf ceux du sol) l'unité est le mm.
	Pour les capteurs noir/blanc (ceux du sol) "1" correspond au blanc parfait et "1000" au noir parfait. Les valeurs intermédiaires sont des nuances de gris.

	Les vitesses sont exprimées en mm/s (il s'agit de la vitesse de variation des valeurs mesurées par chaque capteur)
	Une vitesse positive représente un éloignement vis à vis du robot. A l'inverse une vitesse négative est un raprochement.
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


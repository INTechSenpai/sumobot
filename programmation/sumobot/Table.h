#ifndef _TABLE_h
#define _TABLE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define TAILLES_ROBOTS      100
#define ANGLE_CAPTEUR       0.1745329251994329576
#define MARGE_V				50 // la marge de vitesse pour la detection de la vitesse de rapprochement ou non
#define D_CAPTEUR_SOL		75 // distance en le centre du robot et les capteurs sol
#define ANGLE_CAPTEUR_SOL	0.6283185307179586476 // l'angle entre "l avant" du robot et le capteur avant gauche
#define LIMITE_NB			800		// Limite de décision en "noir" et "blanc"
#define MARGE_COTE			145		// la marge pour etre en mode perdu sur les cotés
#define MARGE_AVANT			165		// la marge pour etre en mode perdu sur l'avant et l'arriere cotés
#define MARGE_COHERENCE		200		// la marge pour que deux valeurs de capteurs adjacents soient acceptées
#define _USE_MATH_DEFINES	
#include "Position.h"
#include "RelativeObstacleMap.h"
#include "Obstacle.h"
#include "math.h"

class Table
{
public :

	Table();
	Table(uint32_t rayonRobot, uint32_t rayonBord, float xR, float yR); 
	// xR et yR pour la position du robot adverse

	uint32_t determinerConditionInitiale(RelativeObstacleMap donneesCapteurs); 
	// retourne 1 si les robots sont dos a dos, 2 si le robot ennemi est a notre droite, 3 si il est a notre gauche

	bool updateObstacleMap(RelativeObstacleMap donneesCapteurs, Position &notrePosition); 
	//met a jour la Position(voir struct) du robot adverse et notre position si les capteurs au sol detecte le bord de table

	ObstacleCercle getRobotAdverse();
	ObstacleCercle getBordDeTable();
	ObstacleMap getObstacleMap();
	float getAngleAbsoluRA();
	float getDistanceAway();

private :

	//float calculOrientation(Position position);
	ObstacleCercle robotAdverse;
	ObstacleCercle bordDeTable;
	bool perdu;
	float angleAbsoluRA; //Angle absolue du robot adverse par rapport au repere de base
	float distanceAway; // la distance entre notre robot et le robot adverse (centre-centre)
};

#endif
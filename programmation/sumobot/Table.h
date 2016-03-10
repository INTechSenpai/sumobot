#ifndef _TABLE_h
#define _TABLE_h

#define TAILLES_ROBOTS      100
#define ANGLE_CAPTEUR       0.1745329251994329576
#define MARGE_V				50 // la marge de vitesse pour la detection de la vitesse de rapprochement ou non
#define D_CAPTEUR_SOL		70 // distance en le centre du robot et les capteurs sol
#define ANGLE_CAPTEUR_SOL	0.6283185307179586476 // l'angle entre "l avant" du robot et le capteur avant gauche
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

	void updateObstacleMap(RelativeObstacleMap donneesCapteurs, Position &notrePosition); 
	//met a jour la Position(voir struct) du robot adverse et notre position si les capteurs au sol detecte le bord de table

	ObstacleCercle getRobotAdverse();
	ObstacleCercle getBordDeTable();
	ObstacleMap getObstacleMap();

private :

	//float calculOrientation(Position position);
	ObstacleCercle robotAdverse;
	ObstacleCercle bordDeTable;
};

#endif
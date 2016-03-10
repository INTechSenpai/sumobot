#ifndef _TABLE_h
#define _TABLE_h

#define TAILLES_ROBOTS      100
#define ANGLE_CAPTEUR       0.1745329251994329576
#define MARGE_V				2 // la marge de vitesse pour la detection de la vitesse de rapprochement ou non
#define _USE_MATH_DEFINES
#include "Position.h"
#include "RelativeObstacleMap.h"
#include "Obstacle.h"
#include "math.h"

class Table
{
public :

	Table();
	Table(uint32_t rayonRobot, uint32_t rayonBord, float xR, float yR); // xR et yR pour la position du robot adverse
	void initialiser(RelativeObstacleMap donneesCapteurs);
	void updateObstacleMap(RelativeObstacleMap donneesCapteurs, Position &notrePosition);
	ObstacleCercle getRobotAdverse();
	ObstacleCercle getBordDeTable();
	ObstacleMap getObstacleMap();

private :

	//float calculOrientation(Position position);
	ObstacleCercle robotAdverse;
	ObstacleCercle bordDeTable;
};

#endif
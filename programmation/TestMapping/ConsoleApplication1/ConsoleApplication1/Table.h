#ifndef _TABLE_h
#define _TABLE_h

#define _USE_MATH_DEFINES
#include "Position.h"
#include "RelativeObstacleMap.h"
#include "Obstacle.h"
#include "math.h"

class Table
{
public :

	Table();
	Table(uint32_t rayonRobot, uint32_t rayonBord);
	void initialiser(RelativeObstacleMap donneesCapteurs);
	void updateObstacleMap(RelativeObstacleMap donneesCapteurs, Position notrePosition);
	ObstacleMap getObstacleMap();

private :

	float calculOrientation(Position position);
	ObstacleCercle robotAdverse;
	ObstacleCercle bordDeTable;
};

#endif
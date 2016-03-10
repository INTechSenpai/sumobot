#ifndef _OBSTACLE_h
#define _OBSTACLE_h

#include "Position.h"
#include "stdint.h"
#include <vector>


struct ObstacleCercle
{
	Position position;
	uint32_t rayon;
	bool obstaclePlein;
};

struct Obstaclerectangle
{
	Position position;
	uint32_t rayon;
	bool obstaclePlein;
};
typedef std::vector <ObstacleCercle> ObstacleMap;

#endif

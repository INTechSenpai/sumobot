#ifndef _OBSTACLE_h
#define _OBSTACLE_h

#include "Position.h"
#include "stdint.h"
#include <vector>


struct ObstacleCercle
{
	Position position;
	uint32_t rayon;
};

struct Obstaclerectangle
{
	Position position;
	uint32_t rayon;
};
typedef std::vector <ObstacleCercle> ObstacleMap;

#endif

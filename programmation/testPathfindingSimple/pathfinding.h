#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <cstdint>
#include "../sumobot/Path.h"
#include "../sumobot/Position.h"
#include "../sumobot/Obstacle.h"


class Pathfinding
{
public:
    Pathfinding();
    Trajectory computePath(Position start, Position pointIntermediaire, Position goal, ObstacleMap map);
};

#endif // PATHFINDING_H

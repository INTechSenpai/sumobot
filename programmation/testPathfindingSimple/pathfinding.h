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
    //pas besoin de préciser l'orientation de start et goal
    Trajectory computePath(Position start, Position pointIntermediaire, Position goal);
    //besoin de préciser l'orientation de start et goal
    Trajectory computePath(Position start, Position goal);
};

#endif // PATHFINDING_H

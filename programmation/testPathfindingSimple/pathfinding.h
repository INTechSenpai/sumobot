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

    //autre trajectoire : se tourne de rotation puis suit une trajecture de rayonCourbure de longueur donnée
    Trajectory computePath(float rotation, float rayonCourbure, float longueur);

    //Quand on est perdu : tourne sur lui meme puis fonce sur l'ennemi
    Trajectory computePathFoncerRobot(Position start, Position goal);
};

#endif // PATHFINDING_H

#ifndef PATHFINDING_H
#define PATHFINDING_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <cstdint>
#include <math.h>
#include "Path.h"
#include "Position.h"
#include "Obstacle.h"


class Pathfinding
{
public:
    Pathfinding();
    //pas besoin de préciser l'orientation de start et goal
    Trajectory computePath(Position &start, Position &pointIntermediaire, Position &goal);
    //besoin de préciser l'orientation de start et goal
    Trajectory computePath(Position &start, Position &goal);

    //autre trajectoire : Tourne de rotation puis suit une trajecture de rayonCourbure de longueur donnée
    Trajectory computePath(float rayonCourbure, float longueur);

    //Quand on est perdu : tourne sur lui meme puis fonce sur l'ennemi
    Trajectory computePathFoncerRobot(Position &start, Position &goal, float longueur);
};

#endif // PATHFINDING_H

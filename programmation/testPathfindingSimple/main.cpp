#include <iostream>
#include "pathfinding.h"
using namespace std;

int main(int argc, char *argv[])
{
    Pathfinding pathfinding;
    Position start, positionIntermediaire, goal;
    ObstacleMap map;
    start.x = 45;
    start.y = 30;
    start.xSpeed = 50;
    start.ySpeed = 50;
    positionIntermediaire.x = 50;
    positionIntermediaire.y = 40;
    goal.x = 60;
    goal.y = 45;
    pathfinding.computePath(start, positionIntermediaire, goal, map);
    return 0;
}

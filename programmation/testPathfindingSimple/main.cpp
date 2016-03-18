#include <iostream>
#include "pathfinding.h"
using namespace std;

int main(int argc, char *argv[])
{
    Pathfinding pathfinding;
    Position start, pointInter, goal;
    start.x = 50;
    start.y = 5;
    start.orientation = 0;
    start.xSpeed = 50;
    start.ySpeed = 50;

    pointInter.x = -71.35;
    pointInter.y = 4.8;

    goal.x = -56.8;
    goal.y = -53.9;
    goal.orientation = 0;
    pathfinding.computePath(start, pointInter, goal);

    pathfinding.computePath(start, goal);

    return 0;
}

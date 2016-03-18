#include <iostream>
#include "pathfinding.h"
using namespace std;

int main(int argc, char *argv[])
{
    Pathfinding pathfinding;
    Position start, pointInter, goal;
    start.x = 50;
    start.y = 0;
    start.orientation = 2.86;
    start.xSpeed = 450;
    start.ySpeed = 450;

    pointInter.x = -97.4;
    pointInter.y = -141;

    goal.x = -223;
    goal.y = -67.68;
    goal.orientation = 0.65;

    Trajectory bitye = pathfinding.computePath(start, pointInter, goal);

    return 0;
}

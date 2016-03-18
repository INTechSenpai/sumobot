#include <iostream>
#include "pathfinding.h"
using namespace std;

int main(int argc, char *argv[])
{
    Pathfinding pathfinding;
    Position start, pointInter, goal;
    start.x = 6;
    start.y = 26;
    start.orientation = 2.86;
    start.xSpeed = 450;
    start.ySpeed = 450;

    pointInter.x = -71.35;
    pointInter.y = 4.8;

    goal.x = 5;
    goal.y = 4;
    goal.orientation = 0.65;

    pathfinding.computePathFoncerRobot(start, goal);

    return 0;
}

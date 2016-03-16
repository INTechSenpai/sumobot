#include <iostream>
#include "pathfinding.h"
using namespace std;

int main(int argc, char *argv[])
{
    Pathfinding pathfinding;
    Position start, goal;
    start.x = 50;
    start.y = 5;
    start.orientation = 3.63;
    start.xSpeed = 50;
    start.ySpeed = 50;
    goal.x = -27.5;
    goal.y = -49.5;
    goal.orientation = 0.52;
    pathfinding.computePath(start, goal);
    return 0;
}

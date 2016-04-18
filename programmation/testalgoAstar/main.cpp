#include "pathfinding.h"


int main(int argc, char *argv[])
{
    Table table;
    Pathfinding pathfinding;

    table.initObstacleMap(GREEN);
    ObstacleMap map= table.getObstacleMap();

    Position start,goal;
    start.orientation = M_PI;
    start.x = 1350;
    start.y = 1150;

    goal.orientation = M_PI;
    goal.x = 1200;
    goal.y = 1000;

    Trajectory trajectoire = pathfinding.computePath(map, start, goal);
    return 0;
}

#include "pathfinding.h"


int main(int argc, char *argv[])
{
    Table table;
    Pathfinding pathfinding;

    table.initObstacleMap(GREEN);
    ObstacleMap map= table.getObstacleMap();

    Position start,goal;
    start.orientation = 3.14;
    start.x = 1350;
    start.y = 1150;
    map.movableInvisible[0].pushIt();

    goal.orientation = -3;
    goal.x = 1200;
    goal.y = 200;

    Trajectory trajectoire = pathfinding.computePath(map, start, goal);
    return 0;
}

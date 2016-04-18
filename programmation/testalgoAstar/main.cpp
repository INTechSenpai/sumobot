#include "pathfinding.h"


int main(int argc, char *argv[])
{
    Table table;
    Pathfinding pathfinding;

    table.initObstacleMap(GREEN);
    ObstacleMap map= table.getObstacleMap();

    Position start,goal;
    start.orientation = 0;
    start.x = 0;
    start.y = 0;

    goal.orientation = 0;
    goal.x = 400;
    goal.y = 200;

    float intermediateOrientation = 3.0;

    Trajectory trajectoire = pathfinding.computePath(map, start, goal, intermediateOrientation);
    std::cout << trajectoire.empty() << std::endl;
    return 0;
}

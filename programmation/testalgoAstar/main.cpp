#include <iostream>
#include "pathfinding.h"
#include <vector>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])
{
    Pathfinding pathfinding;
    ObstacleCercle unObstacle;
    unObstacle.obstaclePlein = true;
    unObstacle.position.x = -300;
    unObstacle.position.y = 0;
    unObstacle.rayon = 100;

    ObstacleCercle BordDeTable;
    BordDeTable.obstaclePlein = false;
    BordDeTable.position.x = 0;
    BordDeTable.position.y = 0;
    BordDeTable.rayon = 740;

    ObstacleMap map;
    map.push_back(unObstacle);
    map.push_back(BordDeTable);

    PositionTrajectoire start,goal;
    start.orientation = 0;
    start.x = 300;
    start.y = 0;

    goal.orientation = M_PI/2;
    goal.x = 0;
    goal.y = 150;

    pathfinding.Astar(map, start, goal);
    return 0;
}

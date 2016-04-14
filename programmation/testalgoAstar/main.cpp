#include <iostream>
#include "pathfinding.h"
#include <vector>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])
{
    Pathfinding pathfinding;
    Position positionUnObstacle;
    positionUnObstacle.x = -300;
    positionUnObstacle.y = 0;
    Obstacle unObstacle(positionUnObstacle, CIRCLE);
    unObstacle.setRadius(100);

    Position positionDeuxiemeObstacle;
    positionDeuxiemeObstacle.x = 0;
    positionDeuxiemeObstacle.y = 0;
    Obstacle deuxiemeObstacle(positionDeuxiemeObstacle, RECTANGLE);
    deuxiemeObstacle.setXRadius(100);
    deuxiemeObstacle.setYRadius(100);


    ObstacleMap map;
    map.movableInvisible.push_back(unObstacle);
    map.fixedVisible.push_back(deuxiemeObstacle);

    PositionTrajectoire start,goal;
    start.orientation = 0;
    start.x = 300;
    start.y = 0;

    goal.orientation = M_PI/2;
    goal.x = 0;
    goal.y = 150;

    pathfinding.Astar(start, goal);
    return 0;
}

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

    Position start,goal;
    start.orientation = 4.05;
    start.x = 300;
    start.y = 0;

    goal.orientation = M_PI/2;
    goal.x = -120;
    goal.y = 0;

    float intermediateOrientation = 3.0;

    pathfinding.computePath(map, start, goal, intermediateOrientation);
    return 0;
}

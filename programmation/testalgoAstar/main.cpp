#include <iostream>
#include "mapdeuxdes.h"
#include "pathfinding.h"
#include <vector>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])
{
    Pathfinding pathfinding;
    ObstacleCercle unObstacle;
    unObstacle.obstaclePlein = true;
    unObstacle.position.x = 0;
    unObstacle.position.y = 0;
    unObstacle.rayon = 20;

    ObstacleCercle BordDeTable;
    BordDeTable.obstaclePlein = false;
    BordDeTable.position.x = 0;
    BordDeTable.position.y = 0;
    BordDeTable.rayon = 740;

    ObstacleMap map;
    map.push_back(unObstacle);
    map.push_back(BordDeTable);

    Position start,goal;
    start.orientation = 0;
    start.x = 100;
    start.y = 0;

    goal.orientation = M_PI;
    goal.x = -100;
    goal.y = 0;

    vector<Position> ResultatAstar = pathfinding.Astar(map, start, goal);

    cout << "chemin proposé :" << endl;
    for (int i=0; i<ResultatAstar.size();i++) {
        cout << "x = " << ResultatAstar[i].x;
        cout << " y = " << ResultatAstar[i].y;
        cout << " orientation = " << ResultatAstar[i].orientation;
        cout << endl;

    }

    return 0;
}

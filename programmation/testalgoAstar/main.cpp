#include <iostream>
#include "mapdeuxdes.h"
#include "pathfinding.h"
using namespace std;

int main(int argc, char *argv[])
{
    Mapdeuxdes environnement;
    environnement.afficherMap();

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    Pathfinding pathfinding;
    std::vector<std::pair<int, int>> solution = pathfinding.Astar(environnement,environnement.getDepart(), environnement.getArrive());
    for (auto i=0; i<solution.size();i++) {
        environnement.marquerMap(solution[i].first, solution[i].second);
    }
    environnement.afficherMap();
    return 0;
}

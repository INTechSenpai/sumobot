#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <utility>
#include "mapdeuxdes.h"
#include <map>
#include <vector>
#define N 10
struct noeud {
    std::pair<int, int> parent;
    double cout_g, cout_h, cout_f;
};

class Pathfinding
{
public:
    Pathfinding();
    std::vector<std::pair<int,int>> Astar(Mapdeuxdes map, std::pair<int,int> start,std::pair<int,int> goal);

private:

    //Contient les noeuds en liste fermée
    std::map<std::pair<int, int>, noeud> ClosedSet;
    //Contient les noeuds en liste ouverte
    std::map<std::pair<int, int>, noeud> OpenSet;

    double distance(int x1, int y1, int x2, int y2);

    void MettreAjourOpenSet(Mapdeuxdes map, std::pair<int,int> start,std::pair<int,int> goal);
    std::pair<int, int> MettreAjourClosedSet();
    bool estDansOpenSet(int i,int j);



};

/* cout_g : distance du noeud au départ
 * cout_h : distance du noeud à l'arrivée
 * cout_f : cout_h + cout_g
 */



#endif // PATHFINDING_H

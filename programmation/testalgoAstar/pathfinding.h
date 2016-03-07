#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <utility>
#include "mapdeuxdes.h"
#include <vector>
#define distanceParEtape 10.0

struct Position
{
    float x;
    float y;
    float orientation;
    float xSpeed;
    float ySpeed;
};

struct noeud {
    Position parent;
    Position position;
    double cout_g, cout_h, cout_f;
};



class Pathfinding
{
public:
    Pathfinding();
    std::vector<Position> Astar(Mapdeuxdes map, Position start, Position goal);

private:

    //Contient les noeuds en liste fermée
    std::vector<noeud> ClosedSet;
    //Contient les noeuds en liste ouverte
    std::vector<noeud> OpenSet;

    double distance(int x1, int y1, int x2, int y2);

    void MettreAjourOpenSet(Mapdeuxdes map, Position start,Position goal);
    Position MettreAjourClosedSet();

    // renvoie la position dans le vecteur si trouvé, -1 sinon (yolo?)

    int chercheDansOpenSet(Position positionAtest);
    int chercheDansClosedSet(Position positionAtest);


    bool estSurUnObstacle(float x, float y);

    //yolo
    bool PosEgales(Position p1, Position p2);

};

/* cout_g : distance du noeud au départ
 * cout_h : distance du noeud à l'arrivée
 * cout_f : cout_h + cout_g
 */



#endif // PATHFINDING_H

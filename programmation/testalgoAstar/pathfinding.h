#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <utility>
#include "mapdeuxdes.h"
#include <vector>
#include "../TestMapping/ConsoleApplication1/ConsoleApplication1/Obstacle.h"
#include "../TestMapping/ConsoleApplication1/ConsoleApplication1/Table.h"
#include "../TestMapping/ConsoleApplication1/ConsoleApplication1/Position.h"

#define distanceParEtape 10.0
#define R1 10.0
#define R2 30.0
#define N 10
/*
struct Position
{
    float x;
    float y;
    float orientation;
    float xSpeed;
    float ySpeed;
};
*/
struct noeud {
    Position parent;
    Position position;
    double cout_g, cout_h, cout_f;
};



class Pathfinding
{
public:
    Pathfinding();
    std::vector<Position> Astar(const ObstacleMap& map, const Position& start, const Position& goal);

private:

    //Contient les noeuds en liste fermée
    std::vector<noeud> ClosedSet;
    //Contient les noeuds en liste ouverte
    std::vector<noeud> OpenSet;

    double distance(int x1, int y1, int x2, int y2);

    void MettreAjourOpenSet(const ObstacleMap& map, const Position& start,const Position& goal);
    void checkCandidat(const Position& candidat, const ObstacleMap &map, const Position& start, const Position& goal);

    Position MettreAjourClosedSet();

    //place dans openset de manière a trier
    void PlacerDansOpenSet(const noeud& nouveauNoeud);

    // renvoie la position dans le vecteur si trouvé, -1 sinon (yolo?)
    int chercheDansOpenSet(const Position& positionAtest);
    int chercheDansClosedSet(const Position& positionAtest);


    bool estSurUnObstacle(float x, float y, const ObstacleMap& map);

    //yolo
    bool PosEgales(const Position &p1, const Position &p2);

    //critère d'arret
    bool PosSuffisammentProches(const Position& p1, const Position& p2);

};

/* cout_g : distance du noeud au départ
 * cout_h : distance du noeud à l'arrivée
 * cout_f : cout_h + cout_g
 */



#endif // PATHFINDING_H

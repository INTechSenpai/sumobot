#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <utility>
#include "mapdeuxdes.h"
#include <vector>
#include "../sumobot/Obstacle.h"
#include "../sumobot/Table.h"
#include "../sumobot/Position.h"
#include "../sumobot/Path.h"

#define distanceParEtape 20.0
#define R1 10.0
#define R2 100.0
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
    float cout_g, cout_h, cout_f;
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

    float distance(float x1, float y1, float o1, float x2, float y2, float o2);

    bool EstUnTrajetImpossible(const ObstacleMap& map, const Position& start, const Position& goal);

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

    Trajectory positionsToTrajectory(const std::vector<Position>& chemin_solution);

};

/* cout_g : distance du noeud au départ
 * cout_h : distance du noeud à l'arrivée
 * cout_f : cout_h + cout_g
 */



#endif // PATHFINDING_H

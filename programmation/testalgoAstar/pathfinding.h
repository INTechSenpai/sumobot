#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <utility>
#include "mapdeuxdes.h"
#include <vector>
#include "../sumobot/Obstacle.h"
#include "../sumobot/Table.h"
#include "../sumobot/Position.h"
#include "../sumobot/Path.h"

#define distanceParEtape 100.0
#define R1 50.0
#define R2 100.0
#define R3 500.0

enum Trajectoire{Depart,
                Avancer1g,Avancer1d,Reculer1g,Reculer1d,
                Avancer2g,Avancer2d,Reculer2g,Reculer2d,
                Avancer3g,Avancer3d,Reculer3g,Reculer3d,
                AvancerToutDroit, ReculerToutDroit, TournerSurPlaceTrigo,
                TournerSurPlaceAntiTrigo};

struct PositionTrajectoire
{
    float x;
    float y;
    float orientation;
    Trajectoire trajectoire;

};

struct noeud {
    PositionTrajectoire parent;
    PositionTrajectoire position;
    float cout_g, cout_h, cout_f;
};



class Pathfinding
{
public:
    Pathfinding();
    Trajectory ComputePath(ObstacleMap& map, const Position& start, const Position& goal, float intermediateOrientation);
    //a passser en private les tests terminés
    Trajectory Astar(const ObstacleMap& map, const PositionTrajectoire& start, const PositionTrajectoire& goal);

private:

    //Contient les noeuds en liste fermée
    std::vector<noeud> ClosedSet;
    //Contient les noeuds en liste ouverte
    std::vector<noeud> OpenSet;

    float distance(float x1, float y1, float o1, float x2, float y2, float o2);

    //bool EstUnTrajetImpossible(const ObstacleMap& map, const PositionTrajectoire& start, const PositionTrajectoire& goal);

    void MettreAjourOpenSet(const ObstacleMap& map, const PositionTrajectoire& start,const PositionTrajectoire& goal);
    void checkCandidat(const PositionTrajectoire& candidat, const ObstacleMap &map, const PositionTrajectoire& start, const PositionTrajectoire& goal);

    PositionTrajectoire MettreAjourClosedSet();

    //place dans openset de manière a trier
    void PlacerDansOpenSet(const noeud& nouveauNoeud);

    // renvoie la position dans le vecteur si trouvé, -1 sinon
    int chercheDansOpenSet(const PositionTrajectoire& positionAtest);
    int chercheDansClosedSet(const PositionTrajectoire& positionAtest);


    bool estSurUnObstacle(float x, float y, const ObstacleMap& map);

    //vérifie si deux positions sont égales
    bool PosEgales(const PositionTrajectoire &p1, const PositionTrajectoire &p2);

    //critère d'arret
    bool PosSuffisammentProches(const PositionTrajectoire& p1, const PositionTrajectoire& p2);

    Trajectory positionsToTrajectory(const std::vector<PositionTrajectoire>& chemin_solution);

};

/* cout_g : distance du noeud au départ
 * cout_h : distance du noeud à l'arrivée
 * cout_f : cout_h + cout_g
 */



#endif // PATHFINDING_H

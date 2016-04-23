#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <vector>
#include <iostream>
#include <math.h>
#include "../sumobot/Obstacle.h"
#include "../sumobot/Table.h"
#include "../sumobot/Position.h"
#include "../sumobot/Path.h"

#define SEUIL_ROTATION 150

class Pathfinding
{

private:

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

public:
    Pathfinding();
    Trajectory computePath(const ObstacleMap& map, const Position& start, const Position& goal);

private:

    Trajectory Astar(const PositionTrajectoire& start, const PositionTrajectoire& goal);

    float distance(float x1, float y1, float o1, float x2, float y2, float o2);

    //bool EstUnTrajetImpossible(const ObstacleMap& map, const PositionTrajectoire& start, const PositionTrajectoire& goal);

    void MettreAjourOpenSet(const PositionTrajectoire& start, const PositionTrajectoire& goal);
    void checkCandidat(const PositionTrajectoire& candidat, const PositionTrajectoire& start, const PositionTrajectoire& goal);

    PositionTrajectoire MettreAjourClosedSet();

    //place dans openset de manière a trier
    void PlacerDansOpenSet(const noeud& nouveauNoeud);

    // renvoie la position dans le vecteur si trouvé, -1 sinon
    int chercheDansOpenSet(const PositionTrajectoire& positionAtest);
    int chercheDansClosedSet(const PositionTrajectoire& positionAtest);

    //renvoie -1 si la position donnée se trouve sur un obstacle
    //sinon renvoie l'entier correspondant a l'obstacle dans obstaclesSurLaMap
    int estSurUnObstacle(float x, float y);

    //vérifie si deux positions sont égales
    bool PosEgales(const PositionTrajectoire &p1, const PositionTrajectoire &p2);

    //critère d'arret
    bool PosSuffisammentProches(const PositionTrajectoire& p1, const PositionTrajectoire& p2, float precisionXY, float precisionOrientation);

    Trajectory positionsToTrajectory(const std::vector<PositionTrajectoire>& chemin_solution);

private:

    //Contient les noeuds en liste fermée
    std::vector<noeud> ClosedSet;
    //Contient les noeuds en liste ouverte
    std::vector<noeud> OpenSet;

    //Contient tous les obstacles dans un seul vecteur
    std::vector<Obstacle> obstaclesSurLaMap;

    //la distance parcourue pour une boucle de l'algorithme
    const float distanceParEtape;

    //coefficient définissant l'importance de l'écart d'orientation
    //par rapport à l'écart de distance euclidienne.
    const float coeffOrientation;

    //contient les différents rayons de courbures possibles
    std::vector<float> rayonsDeCourbures;

    //rotations autorisées si true
    bool rotationAllowed;

    //trajectoires courbes interdites si true
    bool isUsingSimpleTrajectory;


};

/* cout_g : distance du noeud au départ
 * cout_h : distance du noeud à l'arrivée
 * cout_f : cout_h + cout_g
 */



#endif // PATHFINDING_H

#include "pathfinding.h"
#include "mapdeuxdes.h"
#include <vector>
#include <math.h>
#include <iostream>

Pathfinding::Pathfinding(){}

//A tester : distance basée également sur écart d'orientation
float Pathfinding::distance(float x1, float y1, float o1, float x2, float y2, float o2){
    return ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (o1-o2)*(o1-o2)*200);
}

//yolo

bool Pathfinding::PosEgales(const Position& p1, const Position& p2) {
    return (
            (p1.orientation == p2.orientation) &&
            (p1.x == p2.x) &&
            (p1.y == p2.y)
           );
}

bool Pathfinding::PosSuffisammentProches(const Position& p1, const Position& p2) {
    float precisionXY = 10.0;
    float precisionOrientation = 0.5;
    return (
            (p1.orientation < p2.orientation + precisionOrientation) &&
            (p2.orientation - precisionOrientation < p1.orientation) &&

            (p1.x < p2.x + precisionXY) &&
            (p2.x - precisionXY < p1.x) &&

            (p1.y < p2.y + precisionXY) &&
            (p2.y - precisionXY < p1.y)
           );
}



std::vector<Position> Pathfinding::Astar(const ObstacleMap& map, const Position& start, const Position& goal) {

    std::vector<Position> chemin_solution;

    if (EstUnTrajetImpossible(map, start, goal)) {
        return chemin_solution;
    }
    int passageDansBoucle=0;
    Position n_courant = start;
    noeud noeuddepart;
    noeuddepart.cout_g = 0;
    noeuddepart.cout_h = distance(start.x, start.y, start.orientation, goal.x, goal.y, goal.orientation);
    noeuddepart.cout_f = noeuddepart.cout_h;
    noeuddepart.position = n_courant;

    OpenSet.push_back(noeuddepart);
    n_courant = MettreAjourClosedSet();
    while (!(PosSuffisammentProches(n_courant, goal))&&(passageDansBoucle<150)) {
        MettreAjourOpenSet(map, n_courant, goal);
        n_courant = MettreAjourClosedSet();
        passageDansBoucle++;
    }

    if (passageDansBoucle == 150) {
        return chemin_solution;
    }


   //  truc a faire si closedSet est pas trié
    noeud tmp = ClosedSet[chercheDansClosedSet(n_courant)];
    while (!PosEgales(n_courant, start)) {

        chemin_solution.push_back(n_courant);
        tmp = ClosedSet[chercheDansClosedSet(tmp.parent)];

        n_courant.x = tmp.parent.x;
        n_courant.y = tmp.parent.y;
        n_courant.orientation = tmp.parent.orientation;
    }
    chemin_solution.push_back(n_courant);
/* truc a faire si il l'est
    for (int i=0; i<ClosedSet.size();i++) {
        chemin_solution.push_back(ClosedSet[i].position);
    }
    */

    return chemin_solution;
}

bool Pathfinding::EstUnTrajetImpossible(const ObstacleMap& map, const Position& start, const Position& goal) {
    if (map[0].obstaclePlein) {
        return (goal.x*goal.x + goal.y*goal.y > map[1].rayon*map[1].rayon);

    }
    else {
        return (goal.x*goal.x + goal.y*goal.y > map[0].rayon*map[0].rayon);
    }
}


void Pathfinding::MettreAjourOpenSet(const ObstacleMap &map, const Position& start, const Position& goal) {

    // on ajoute à la liste ouverte les noeuds voisins et on calcule leurs coûts

    Position tmp;

    //cas ou on avance en ligne droite de distanceParEtape

    tmp.x = start.x + distanceParEtape*cos(start.orientation);
    tmp.y = start.y + distanceParEtape*sin(start.orientation);
    tmp.orientation = start.orientation;
    tmp.xSpeed = 0;
    tmp.ySpeed = 0;

    checkCandidat(tmp, map, start, goal);

    //cas où on avance de distanceParEtape suivant un rayon R1 vers la gauche.

    tmp.orientation = start.orientation + (distanceParEtape/R1);
    tmp.x = ( start.x - R1*sin(start.orientation) ) - R1*sin(tmp.orientation);
    tmp.y = (start.y + R1*cos(start.orientation)) + R1*sin(tmp.orientation);

    checkCandidat(tmp, map, start, goal);

    //cas où on avance de distanceParEtape suivant un rayon R1 vers la droite.

    tmp.orientation = start.orientation - (distanceParEtape/R1);
    tmp.x = ( start.x + R1*sin(start.orientation) ) + R1*sin(tmp.orientation);
    tmp.y = (start.y - R1*cos(start.orientation)) - R1*sin(tmp.orientation);


    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R1 par la gauche.

    tmp.orientation = start.orientation - (distanceParEtape/R1);
    tmp.x = ( start.x - R1*sin(start.orientation) ) - R1*sin(tmp.orientation);
    tmp.y = (start.y + R1*cos(start.orientation)) + R1*sin(tmp.orientation);

    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R1 par la droite.

    tmp.orientation = start.orientation + (distanceParEtape/R1);
    tmp.x = ( start.x + R1*sin(start.orientation) ) + R1*sin(tmp.orientation);
    tmp.y = (start.y - R1*cos(start.orientation)) - R1*sin(tmp.orientation);

    checkCandidat(tmp, map, start, goal);

    //cas où on avance de distanceParEtape suivant un rayon R2 vers la gauche.

    tmp.orientation = start.orientation + (distanceParEtape/R2);
    tmp.x = ( start.x - R2*sin(start.orientation) ) - R2*sin(tmp.orientation);
    tmp.y = (start.y + R2*cos(start.orientation)) + R2*sin(tmp.orientation);

    checkCandidat(tmp, map, start, goal);

    //cas où on avance de distanceParEtape suivant un rayon R2 vers la droite.

    tmp.orientation = start.orientation - (distanceParEtape/R2);
    tmp.x = ( start.x + R2*sin(start.orientation) ) + R2*sin(tmp.orientation);
    tmp.y = (start.y - R2*cos(start.orientation)) - R2*sin(tmp.orientation);


    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R2 par la gauche.

    tmp.orientation = start.orientation - (distanceParEtape/R2);
    tmp.x = ( start.x - R2*sin(start.orientation) ) - R2*sin(tmp.orientation);
    tmp.y = (start.y + R2*cos(start.orientation)) + R2*sin(tmp.orientation);

    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R2 par la droite.

    tmp.orientation = start.orientation + (distanceParEtape/R2);
    tmp.x = ( start.x + R2*sin(start.orientation) ) + R2*sin(tmp.orientation);
    tmp.y = (start.y - R2*cos(start.orientation)) - R2*sin(tmp.orientation);

    checkCandidat(tmp, map, start, goal);

}

void Pathfinding::checkCandidat(const Position& candidat, const ObstacleMap& map, const Position& start, const Position& goal) {

    //on vérifie la présence d'Obstacle

    if (!estSurUnObstacle(candidat.x,candidat.y, map)) {
        //on vérifie la présence dans la liste fermée.
        if (chercheDansClosedSet(candidat)==-1) {
            // Création du noeud
            noeud nouveauNoeud;

            // son parent est start et on peut alors calculer son cout.

            nouveauNoeud.parent = start;
            nouveauNoeud.cout_g = ClosedSet.front().cout_g + distance(start.x, start.y, start.orientation,
                                                                      candidat.x, candidat.y, candidat.orientation);
            nouveauNoeud.cout_h = distance(goal.x, goal.y, goal.orientation,
                                           candidat.x, candidat.y, candidat.orientation);
            nouveauNoeud.cout_f = nouveauNoeud.cout_h + nouveauNoeud.cout_g;
            nouveauNoeud.position = candidat;

            //on va chercher l'élément dans OpenSet (si on ne le trouve pas, element = -1)

            int positionNouveauN = chercheDansOpenSet(nouveauNoeud.position);

            // Si l'élément n'est pas dans la liste ouverte, on peut ajouter le noeud
            if (positionNouveauN == -1) {
                PlacerDansOpenSet(nouveauNoeud);
            }

            // Si l'élément est déjà dans la liste ouverte
            // on regarde si le cout est plus bas et si oui
            // on change le cout et son parent

            else if (OpenSet[positionNouveauN].cout_f > nouveauNoeud.cout_f) {
                    OpenSet[positionNouveauN] = nouveauNoeud;
               }

            }
    }
}



Position Pathfinding::MettreAjourClosedSet() {
    // Recherche du minimum des coûts dans OpenSet
    // ici le vector est trié donc on prend le premier élément

    //ajout de ce noeud dans la liste fermée
    //tri le closed set en fonction des parents du noeud
    /*
    int i=0;
    while ((i<ClosedSet.size())&&(!PosEgales(OpenSet.front().parent, ClosedSet[i].position))) {
        i++;
    }
    ClosedSet.insert(ClosedSet.begin()+i,OpenSet.front());
    */

    noeud min_noeud = OpenSet.front();
    ClosedSet.push_back(min_noeud);
    // il faut le supprimer de la liste ouverte, ce n'est plus une solution explorable
    OpenSet.erase(OpenSet.begin());
    return min_noeud.position;
}

bool Pathfinding::estSurUnObstacle(float x, float y, const ObstacleMap &map) {

    for (int i=0;i<map.size();i++) {

        if (map[i].obstaclePlein) {
            if ( (map[i].position.x - x)*(map[i].position.x - x) +
                  (map[i].position.y - y)*(map[i].position.y - y)
                  < map[i].rayon
                    ) {
                return true;
            }
        }

        else {
            if ( (map[i].position.x - x)*(map[i].position.x - x) +
                  (map[i].position.y - y)*(map[i].position.y - y)
                  > map[i].rayon*map[i].rayon
                    ) {
                return true;
            }
        }
     }

    return false;
}

void Pathfinding::PlacerDansOpenSet(const noeud& nouveauNoeud) {
    int i=0;
    while ((i<OpenSet.size())&&(nouveauNoeud.cout_f>OpenSet[i].cout_f)) {
        i++;
    }
    OpenSet.insert(OpenSet.begin()+i,nouveauNoeud);

}

int Pathfinding::chercheDansOpenSet(const Position& positionAtest) {
    if (OpenSet.empty()) {
        return -1;
    }

    else {
        int i=0;
        while (
               (i<OpenSet.size()) &&
               (!PosEgales(OpenSet[i].position, positionAtest)) ) {

            i++;
        }
        if (i<OpenSet.size())
            return i;
        else
            //on retourne -1 si l'on a rien trouvé.
            return -1;


    }

}

int Pathfinding::chercheDansClosedSet(const Position& positionAtest) {
    if (ClosedSet.empty()) {
        return -1;
    }

    else {
        int i=0;
        while (
               (i<ClosedSet.size()) &&
               (!PosEgales(ClosedSet[i].position, positionAtest)) ) {

            i++;
        }
        if (i<ClosedSet.size())
            return i;
        else
            //on retourne -1 si l'on a rien trouvé.
            return -1;

    }
}

#include "pathfinding.h"
#include "mapdeuxdes.h"
#include <vector>
#include <math.h>
#include <iostream>

Pathfinding::Pathfinding(){}

double Pathfinding::distance(int x1, int y1, int x2, int y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

//yolo

bool Pathfinding::PosEgales(Position p1, Position p2) {
    return (
            (p1.orientation == p2.orientation) &&
            (p1.x == p2.x) &&
            (p1.y == p2.y)
           );
}

std::vector<Position> Pathfinding::Astar(Mapdeuxdes map, Position start, Position goal) {

    Position n_courant = start;
    noeud noeuddepart;
    noeuddepart.cout_g = 0;
    noeuddepart.cout_h = distance(start.x, start.y, goal.x, goal.y);
    noeuddepart.cout_f = noeuddepart.cout_h;
    noeuddepart.position = n_courant;

    OpenSet.push_back(noeuddepart);
    n_courant = MettreAjourClosedSet();
    while (!PosEgales(n_courant, goal)) {
        MettreAjourOpenSet(map, n_courant, goal);
        n_courant = MettreAjourClosedSet();
    }
    std::vector<Position> chemin_solution;

    noeud tmp = ClosedSet.back(); // doutes
    Position n;
    n.x = tmp.parent.x;
    n.y = tmp.parent.y;
    n.orientation = tmp.parent.orientation;

    while (!PosEgales(n, start)) {

        chemin_solution.push_back(n);
        tmp = ClosedSet[chercheDansClosedSet(tmp.parent)];

        n.x = tmp.parent.x;
        n.y = tmp.parent.y;
        n.orientation = tmp.parent.orientation;
    }
    return chemin_solution;
}


void Pathfinding::MettreAjourOpenSet(Mapdeuxdes map, Position start, Position goal) {

    // on ajoute à la liste ouverte les noeuds voisins et on calcule leurs coûts

    //cas ou on avance en ligne droite de distanceParEtape

    Position tmp;
    tmp.y = start.y + distanceParEtape*sin(start.orientation);
    tmp.x = start.x + distanceParEtape*cos(start.orientation);
    tmp.orientation = start.orientation;
    tmp.xSpeed = 0;
    tmp.ySpeed = 0;

    //on vérifie que ça ne dépasse pas (à modifier)
    if ( ((tmp.x>=0)&&(tmp.x<N)) && ((tmp.y>=0)&&(tmp.y<N)) ) {

        //on vérifie la présence d'obstacle
        if (!estSurUnObstacle(tmp.x,tmp.y)) {
            //on vérifie la présence dans la liste fermée.
            if (chercheDansClosedSet(tmp)==-1) {
                // Création du noeud
                noeud nouveauNoeud;

                // son parent est start et on peut alors calculer son cout.

                nouveauNoeud.parent = start;
                nouveauNoeud.cout_g = ClosedSet.front().cout_g + distance(start.x, start.y, tmp.x, tmp.y);
                nouveauNoeud.cout_h = distance(goal.x, goal.y, tmp.x, tmp.y);
                nouveauNoeud.cout_f = nouveauNoeud.cout_h + nouveauNoeud.cout_g;
                nouveauNoeud.position = tmp;

                //on va chercher l'élément dans OpenSet (si on ne le trouve pas, element = -1)

                int positionNouveauN = chercheDansOpenSet(nouveauNoeud.position);

                // Si l'élément n'est pas dans la liste ouverte, on peut ajouter le noeud
                if (positionNouveauN == -1) {
                    OpenSet.push_back(nouveauNoeud);
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
}

Position Pathfinding::MettreAjourClosedSet() {
    // Recherche du minimum des coûts dans OpenSet

    std::vector<noeud>::iterator min_noeud = OpenSet.begin();

    for (std::vector<noeud>::iterator i = OpenSet.begin();i!=OpenSet.end();i++) {
        if (i->cout_f < min_noeud->cout_f) {
            min_noeud = i;
        }
    }

    //ajout de ce noeud dans la liste fermée
    ClosedSet.push_back(*min_noeud);

    // il faut le supprimer de la liste ouverte, ce n'est plus une solution explorable
    OpenSet.erase(min_noeud);
    return min_noeud->position;
}

bool Pathfinding::estSurUnObstacle(float x, float y) {
    return false;
}

int Pathfinding::chercheDansOpenSet(Position positionAtest) {
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

int Pathfinding::chercheDansClosedSet(Position positionAtest) {
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

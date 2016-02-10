#include "pathfinding.h"
#include "mapdeuxdes.h"
#include <map>
#include <math.h>
#include <iostream>

Pathfinding::Pathfinding(){
}

double Pathfinding::distance(int x1, int y1, int x2, int y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

std::vector<std::pair<int,int>> Pathfinding::Astar(Mapdeuxdes map, std::pair<int,int> start,std::pair<int,int> goal) {

    std::pair<int, int> n_courant(start.first, start.second);
    noeud noeuddepart;
    noeuddepart.cout_g = 0;
    noeuddepart.cout_h = distance(start.first, start.second, goal.first, goal.second);
    noeuddepart.cout_f = noeuddepart.cout_h;
    OpenSet[start] = noeuddepart;
    n_courant = MettreAjourClosedSet();
    while (n_courant != goal) {
        MettreAjourOpenSet(map, n_courant, goal);
        n_courant = MettreAjourClosedSet();
    }
    std::vector<std::pair<int, int>> chemin_solution;

    noeud tmp = ClosedSet[goal];
    std::pair<int, int> n;
    n.first = tmp.parent.first;
    n.second = tmp.parent.second;

    while (n != start) {
        chemin_solution.emplace_back(n);
        tmp = ClosedSet[tmp.parent] ;
        n.first = tmp.parent.first;
        n.second = tmp.parent.second;
    }
    return chemin_solution;
}


void Pathfinding::MettreAjourOpenSet(Mapdeuxdes map, std::pair<int, int> start, std::pair<int, int> goal) {

    // on ajoute à la liste ouverte les noeuds voisins et on calcule leurs coûts
    for (int i = start.first-1; i<=start.first+1;i++) {
        if ((i >=0)&&(i<N)) {
            for (int j = start.second-1;j<=start.second+1;j++) {


                // Il faut que le noeud voisin ne dépasse pas de la map, ne soit pas un obstacle, et
                // ne soit pas le noeud de départ.

                if (((i!=start.first)||(j!=start.second))&&(!map.estUnObstacle(i,j))&&(j>=0)&&(j<N)) {
                   //Il faut que l'élément ne soit pas dans la liste fermée.

                    if (ClosedSet.find(std::pair<int, int>(i,j)) == ClosedSet.end()) {
                        // Création du noeud
                        noeud nouveauNoeud;

                        // son parent est start et on peut alors calculer son cout.

                        nouveauNoeud.parent = start;
                        nouveauNoeud.cout_g = ClosedSet[start].cout_g + distance(start.first, start.second, i, j);
                        nouveauNoeud.cout_h = distance(goal.first, goal.second, i, j);
                        nouveauNoeud.cout_f = nouveauNoeud.cout_h + nouveauNoeud.cout_g;


                        // Si l'élément n'est pas dans la liste ouverte, on peut ajouter le noeud
                        if (!estDansOpenSet(i,j)) {
                            OpenSet[std::pair<int, int>(i,j)] = nouveauNoeud;
                        }

                        // Si l'élément est déjà dans la liste ouverte
                        // on regarde si le cout est plus bas et si oui
                        // on change le cout et son parent

                        else if (OpenSet[std::pair<int, int>(i,j)].cout_f > nouveauNoeud.cout_f) {
                                OpenSet[std::pair<int, int>(i,j)] = nouveauNoeud;
                           }

                   }
                }
            }
        }
    }
}

bool Pathfinding::estDansOpenSet(int i, int j) {
    std::pair<int, int> paireAtest(i,j);
    std::map<std::pair<int, int>, noeud>::iterator it;
    it = OpenSet.find(paireAtest);
    return (it!=OpenSet.end());
}

std::pair<int, int> Pathfinding::MettreAjourClosedSet() {
    // Recherche du minimum des coûts dans OpenSet
    double min_coutf = OpenSet.begin()->second.cout_f;

    std::pair<int, int> min_noeud = OpenSet.begin()->first;

    for (std::map<std::pair<int, int>, noeud>::iterator i = OpenSet.begin();i!=OpenSet.end();i++) {
        if (i->second.cout_f < min_coutf) {
            min_coutf = i->second.cout_f;
            min_noeud = i->first;
        }
    }

    //ajout de ce noeud dans la liste fermée
    ClosedSet[min_noeud] = OpenSet[min_noeud];

    // il faut le supprimer de la liste ouverte, ce n'est plus une solution explorable
    if (OpenSet.erase(min_noeud)==0)
        std::cerr << "Erreur, le noeud n'apparait pas dans la liste ouverte, impossible à supprimer" << std::endl;
    return min_noeud;
}

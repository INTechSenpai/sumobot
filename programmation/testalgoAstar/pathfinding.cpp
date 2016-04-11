#include "pathfinding.h"
#include "mapdeuxdes.h"
#include <vector>
#include <math.h>
#include <iostream>

Pathfinding::Pathfinding(){}

//A tester : distance basée également sur écart d'orientation
float Pathfinding::distance(float x1, float y1, float o1, float x2, float y2, float o2){
    return ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (o1-o2)*(o1-o2)*10000);
}


bool Pathfinding::PosEgales(const PositionTrajectoire& p1, const PositionTrajectoire& p2) {
    return (
            (p1.orientation == p2.orientation) &&
            (p1.x == p2.x) &&
            (p1.y == p2.y)
           );
}

bool Pathfinding::PosSuffisammentProches(const PositionTrajectoire& p1, const PositionTrajectoire& p2) {
    float precisionXY = 20.0;
    float precisionOrientation = 0.3;
    return (
            (p1.orientation < p2.orientation + precisionOrientation) &&
            (p2.orientation - precisionOrientation < p1.orientation) &&

            (p1.x < p2.x + precisionXY) &&
            (p2.x - precisionXY < p1.x) &&

            (p1.y < p2.y + precisionXY) &&
            (p2.y - precisionXY < p1.y)
           );
}



Trajectory Pathfinding::Astar(const ObstacleMap& map, const PositionTrajectoire& start, const PositionTrajectoire& goal) {

    std::vector<PositionTrajectoire> chemin_solution;

    Trajectory trajectoire;
    if (EstUnTrajetImpossible(map, start, goal)) {
        return trajectoire;
    }
    int passageDansBoucle=0;
    PositionTrajectoire n_courant = start;
    n_courant.trajectoire = Depart;
    noeud noeuddepart;
    noeuddepart.cout_g = 0;
    noeuddepart.cout_h = distance(start.x, start.y, start.orientation, goal.x, goal.y, goal.orientation);
    noeuddepart.cout_f = noeuddepart.cout_h;
    noeuddepart.position = n_courant;

    OpenSet.push_back(noeuddepart);
    n_courant = MettreAjourClosedSet();
    while (!(PosSuffisammentProches(n_courant, goal))&&(passageDansBoucle<300)) {
        MettreAjourOpenSet(map, n_courant, goal);
        n_courant = MettreAjourClosedSet();
        passageDansBoucle++;
    }

    if (passageDansBoucle == 300) {
        return trajectoire;
    }


   //  truc a faire si closedSet est pas trié
    noeud tmp = ClosedSet[chercheDansClosedSet(n_courant)];
    while (!PosEgales(n_courant, start)) {
        chemin_solution.push_back(n_courant);
        tmp = ClosedSet[chercheDansClosedSet(tmp.parent)];

        n_courant.x = tmp.position.x;
        n_courant.y = tmp.position.y;
        n_courant.orientation = tmp.position.orientation;
        n_courant.trajectoire = tmp.position.trajectoire;
    }
    chemin_solution.push_back(n_courant);
/* truc a faire si il l'est
    for (int i=0; i<ClosedSet.size();i++) {
        chemin_solution.push_back(ClosedSet[i].position);
    }
    */

    //la vitesse du trajet sera égale a celle d'arrivée
    chemin_solution.front().xSpeed = goal.xSpeed;
    chemin_solution.front().ySpeed = goal.ySpeed;


    //affichage pour debug

    std::cout << "chemin proposé :" << std::endl;
    for (int i=0; i<chemin_solution.size();i++) {
        std::cout << "(" << chemin_solution[i].x;
        std::cout << "," << chemin_solution[i].y;
        std::cout << ") orientation = " << chemin_solution[i].orientation;
        std::cout << " type de trajectoire : " << chemin_solution[i].trajectoire;
        std::cout << std::endl;
    }

    trajectoire = positionsToTrajectory(chemin_solution);
    return (trajectoire);
}

bool Pathfinding::EstUnTrajetImpossible(const ObstacleMap& map, const PositionTrajectoire& start, const PositionTrajectoire& goal) {
    if (map[0].obstaclePlein) {
        return (goal.x*goal.x + goal.y*goal.y > map[1].rayon*map[1].rayon);

    }
    else if (map[1].obstaclePlein){
        return (goal.x*goal.x + goal.y*goal.y > map[0].rayon*map[0].rayon);
    }
    else {
        return true;
    }
}


void Pathfinding::MettreAjourOpenSet(const ObstacleMap &map, const PositionTrajectoire& start, const PositionTrajectoire& goal) {

    // on ajoute à la liste ouverte les noeuds voisins et on calcule leurs coûts

    PositionTrajectoire tmp;


    //cas où on avance de distanceParEtape suivant un rayon R1 vers la gauche.

    float alpha, beta, distanceStartTmp;

    alpha = distanceParEtape/(2*R1);
    beta = alpha + start.orientation;
    distanceStartTmp = 2*R1*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);
    tmp.trajectoire = Avancer1g;


    checkCandidat(tmp, map, start, goal);

    //cas où on avance de distanceParEtape suivant un rayon R1 vers la droite.

    alpha = distanceParEtape/(2*R1);
    beta = start.orientation - alpha;
    distanceStartTmp = 2*R1*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation - 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);
    tmp.trajectoire = Avancer1d;

    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R1 par la gauche.

    float startOrientationRetourne;

    alpha = distanceParEtape/(2*R1);
    startOrientationRetourne = start.orientation + M_PI;
    beta = startOrientationRetourne - alpha;
    distanceStartTmp = 2*R1*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);

    tmp.trajectoire = Reculer1g;

    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R1 par la droite.

    alpha = distanceParEtape/(2*R1);
    startOrientationRetourne = start.orientation + M_PI;
    beta = startOrientationRetourne + alpha;
    distanceStartTmp = 2*R1*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);

    tmp.trajectoire = Reculer1d;

    checkCandidat(tmp, map, start, goal);

    //cas où on avance de distanceParEtape suivant un rayon R2 vers la gauche.

    alpha = distanceParEtape/(2*R2);
    beta = alpha + start.orientation;
    distanceStartTmp = 2*R2*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);
    tmp.trajectoire = Avancer2g;


    checkCandidat(tmp, map, start, goal);

    //cas où on avance de distanceParEtape suivant un rayon R2 vers la droite.

    alpha = distanceParEtape/(2*R2);
    beta = start.orientation - alpha;
    distanceStartTmp = 2*R2*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation - 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);
    tmp.trajectoire = Avancer2d;

    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R2 par la gauche.

    alpha = distanceParEtape/(2*R2);
    startOrientationRetourne = start.orientation + M_PI;
    beta = startOrientationRetourne - alpha;
    distanceStartTmp = 2*R2*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);

    tmp.trajectoire = Reculer2g;

    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R2 par la droite.

    alpha = distanceParEtape/(2*R2);
    startOrientationRetourne = start.orientation + M_PI;
    beta = startOrientationRetourne + alpha;
    distanceStartTmp = 2*R2*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);

    tmp.trajectoire = Reculer2d;

    checkCandidat(tmp, map, start, goal);



    //cas où on avance de distanceParEtape suivant un rayon R3 vers la gauche.


    alpha = distanceParEtape/(2*R3);
    beta = alpha + start.orientation;
    distanceStartTmp = 2*R3*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);
    tmp.trajectoire = Avancer3g;


    checkCandidat(tmp, map, start, goal);

    //cas où on avance de distanceParEtape suivant un rayon R3 vers la droite.

    alpha = distanceParEtape/(2*R3);
    beta = start.orientation - alpha;
    distanceStartTmp = 2*R3*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation - 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);
    tmp.trajectoire = Avancer3d;

    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R3 par la gauche.


    alpha = distanceParEtape/(2*R3);
    startOrientationRetourne = start.orientation + M_PI;
    beta = startOrientationRetourne - alpha;
    distanceStartTmp = 2*R3*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);

    tmp.trajectoire = Reculer3g;

    checkCandidat(tmp, map, start, goal);

    //cas où on recule de distanceParEtape suivant un rayon R3 par la droite.

    alpha = distanceParEtape/(2*R3);
    startOrientationRetourne = start.orientation + M_PI;
    beta = startOrientationRetourne + alpha;
    distanceStartTmp = 2*R3*cos(M_PI/2 - alpha);

    tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


    tmp.x = start.x + distanceStartTmp*cos(beta);
    tmp.y = start.y + distanceStartTmp*sin(beta);

    tmp.trajectoire = Reculer3d;

    checkCandidat(tmp, map, start, goal);

    //cas ou on avance en ligne droite de distanceParEtape

    tmp.x = start.x + distanceParEtape*cos(start.orientation);
    tmp.y = start.y + distanceParEtape*sin(start.orientation);
    tmp.orientation = start.orientation;
    tmp.xSpeed = 0;
    tmp.ySpeed = 0;
    tmp.trajectoire = AvancerToutDroit;

    checkCandidat(tmp, map, start, goal);

    //cas ou on recule en ligne droite de distanceParEtape

    tmp.x = start.x - distanceParEtape*cos(start.orientation);
    tmp.y = start.y - distanceParEtape*sin(start.orientation);
    tmp.orientation = start.orientation;
    tmp.xSpeed = 0;
    tmp.ySpeed = 0;
    tmp.trajectoire = ReculerToutDroit;

    checkCandidat(tmp, map, start, goal);

    //cas où on tourne de distanceParEtape/R3 dans le sens trigonométrique.

    tmp.orientation = start.orientation + (distanceParEtape/R3);
    tmp.x = start.x;
    tmp.y = start.y;
    tmp.xSpeed = 0;
    tmp.ySpeed = 0;
    tmp.trajectoire = TournerSurPlaceTrigo;


    checkCandidat(tmp, map, start, goal);

    //cas où on tourne de distanceParEtape/R3 dans le sens horaire.

    tmp.orientation = start.orientation - (distanceParEtape/R3);
    tmp.x = start.x;
    tmp.y = start.y;
    tmp.xSpeed = 0;
    tmp.ySpeed = 0;
    tmp.trajectoire = TournerSurPlaceTrigo;


    checkCandidat(tmp, map, start, goal);

}

void Pathfinding::checkCandidat(const PositionTrajectoire& candidat, const ObstacleMap& map, const PositionTrajectoire& start, const PositionTrajectoire& goal) {

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



PositionTrajectoire Pathfinding::MettreAjourClosedSet() {
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

int Pathfinding::chercheDansOpenSet(const PositionTrajectoire& positionAtest) {
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

int Pathfinding::chercheDansClosedSet(const PositionTrajectoire& positionAtest) {
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

Trajectory Pathfinding::positionsToTrajectory(const std::vector<PositionTrajectoire>& chemin_solution) {

    Trajectory trajectoire;

    for (int i=0;i<chemin_solution.size();i++) {
        UnitMove unitmove;
        switch (chemin_solution[i].trajectoire) {

        case Depart:
            break;
        case Avancer1g:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(R1);
            unitmove.setSpeedMm_S(400);
            break;

        case Avancer1d:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(-1*R1);
            unitmove.setSpeedMm_S(400);
            break;

        case Reculer1g:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(-1*R1);
            unitmove.setSpeedMm_S(400);
            break;

        case Reculer1d:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(-1*distanceParEtape);
            unitmove.setBendRadiusMm(-1*R1);
            unitmove.setSpeedMm_S(400);
            break;

        case Avancer2g:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(R2);
            unitmove.setSpeedMm_S(400);
            break;

        case Avancer2d:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(-1*R2);
            unitmove.setSpeedMm_S(400);
            break;

        case Reculer2g:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(-1*R2);
            unitmove.setSpeedMm_S(400);
            break;

        case Reculer2d:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(-1*distanceParEtape);
            unitmove.setBendRadiusMm(-1*R2);
            unitmove.setSpeedMm_S(400);
            break;

        case Avancer3g:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(R3);
            unitmove.setSpeedMm_S(400);
            break;

        case Avancer3d:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(-1*R3);
            unitmove.setSpeedMm_S(400);
            break;

        case Reculer3g:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(-1*R3);
            unitmove.setSpeedMm_S(400);
            break;

        case Reculer3d:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(-1*distanceParEtape);
            unitmove.setBendRadiusMm(-1*R3);
            unitmove.setSpeedMm_S(400);
            break;

        case AvancerToutDroit:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape);
            unitmove.setBendRadiusMm(INFINITE_RADIUS);
            unitmove.setSpeedMm_S(450);
            break;

        case ReculerToutDroit:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(-distanceParEtape);
            unitmove.setBendRadiusMm(INFINITE_RADIUS);
            unitmove.setSpeedMm_S(450);
            break;

        case TournerSurPlaceTrigo:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(distanceParEtape/R3);
            unitmove.setBendRadiusMm(0);
            unitmove.setSpeedMm_S(400);
            break;

        case TournerSurPlaceAntiTrigo:
            unitmove.stopAfterMove = false;
            unitmove.setLengthMm(-distanceParEtape/R3);
            unitmove.setBendRadiusMm(0);
            unitmove.setSpeedMm_S(400);
            break;
        }

        trajectoire.push_back(unitmove);
    }
    return trajectoire;
}

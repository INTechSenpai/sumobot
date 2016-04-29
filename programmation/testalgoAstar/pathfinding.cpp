#include "pathfinding.h"
#define ESTSURORDI

Pathfinding::Pathfinding() : distanceParEtape(100.0), coeffOrientation(1000),
                             rotationAllowed(true), isUsingSimpleTrajectory(false) {

    rayonsDeCourbures.push_back(150.0);
    rayonsDeCourbures.push_back(500.0);
}


Trajectory Pathfinding::computePath(const ObstacleMap& map, const Position& start, const Position& goal) {

    //on autorise les rotations par défaut
    rotationAllowed = true;

    //transforme Position en PositionTrajectoire
    PositionTrajectoire Astart;
    Astart.orientation = fmod(start.orientation, 2*M_PI);
    Astart.x = start.x;
    Astart.y = start.y;
    Astart.trajectoire = Depart;

    PositionTrajectoire Agoal;
    Agoal.orientation = fmod(goal.orientation, 2*M_PI);
    Agoal.x = goal.x;
    Agoal.y = goal.y;
    Agoal.trajectoire = Depart;

    //Transforme l'obstacleMap en un seul vector

    //agrandir les obstacles en prenant en compte que le robot est un cercle de rayon rayonRobot
    obstaclesSurLaMap.insert(obstaclesSurLaMap.end(), map.fixedInvisible.begin(), map.fixedInvisible.end());
    obstaclesSurLaMap.insert(obstaclesSurLaMap.end(), map.fixedVisible.begin(), map.fixedVisible.end());
    obstaclesSurLaMap.insert(obstaclesSurLaMap.end(), map.movableInvisible.begin(), map.movableInvisible.end());
    obstaclesSurLaMap.insert(obstaclesSurLaMap.end(), map.movableVisible.begin(), map.movableVisible.end());
    obstaclesSurLaMap.insert(obstaclesSurLaMap.end(), map.oponentRobot.begin(), map.oponentRobot.end());
    obstaclesSurLaMap.insert(obstaclesSurLaMap.end(), map.toBeSpecified.begin(), map.toBeSpecified.end());

    // On agrandit les obstacles pour prendre en compte la taille du robot
    for (int i=0;i<obstaclesSurLaMap.size();i++) {
        if (obstaclesSurLaMap[i].getShape()==CIRCLE) {
            obstaclesSurLaMap[i].setRadius(obstaclesSurLaMap[i].getRadius()+100);
        }
        else if(obstaclesSurLaMap[i].getShape()==RECTANGLE) {
            obstaclesSurLaMap[i].setXRadius(obstaclesSurLaMap[i].getXRadius()+100);
            obstaclesSurLaMap[i].setYRadius(obstaclesSurLaMap[i].getYRadius()+100);
        }
        else {
            obstaclesSurLaMap[i].setXRadius(obstaclesSurLaMap[i].getXRadius()-100);
            obstaclesSurLaMap[i].setYRadius(obstaclesSurLaMap[i].getYRadius()-100);
        }
    }

    //Détermine si on est sur un obstacle
    int numeroObstacle = estSurUnObstacle(start.x, start.y);
    if (numeroObstacle != -1) {
        if (numeroObstacle < map.fixedInvisible.size() + map.fixedVisible.size()) {
            //si l'obstacle est fixed, rajouter une trajectoire pour s'éloigner

        }
        else {
            //si pas fixed, on l'enleve de nos obstacles
            obstaclesSurLaMap.erase(obstaclesSurLaMap.begin()+numeroObstacle);
        }
    }

#ifdef ESTSURORDI
    //Détermine si on est sur un obstacle
    int goalNumeroObstacle = estSurUnObstacle(goal.x, goal.y);
    if(goalNumeroObstacle != -1) {
        std::cout << "le goal est un obstacle non poussable";
    }
#endif

    //Détermine si il faut passer par un obstacle

    if (Obstacle::isAnObstaclePushed()) {

    //si oui, on trouve l'obstacle à pousser
        Obstacle obstacleApousser = obstaclesSurLaMap[0];
        int i = 1;
        while ((i<obstaclesSurLaMap.size())&&(!obstacleApousser.isPushed())) {

            if (obstaclesSurLaMap[i].isPushed()) {
                obstacleApousser = obstaclesSurLaMap[i];
            }
            i++;
        }

        //récupération de la position de l'obstacle
        Position obstaclePositionT;
        obstacleApousser.getCenter(obstaclePositionT);

        //création de la PositionTrajectoire de l'obstacle
        PositionTrajectoire obstaclePosition;
        obstaclePosition.orientation = obstaclePositionT.orientation;
        obstaclePosition.x = obstaclePositionT.x;
        obstaclePosition.y = obstaclePositionT.y;



        //puis appel de l'algorithme A* deux fois et concaténation du résultat
        Trajectory startToObject = Astar(Astart, obstaclePosition);

        //deuxième appel en évitant les rotations trop importantes

        rotationAllowed = false;
        Trajectory objectToGoal = Astar(obstaclePosition, Agoal);

        //si startToObject vide (A* ne renvoie rien), meme si goalToObject
        //renvoie une trajectoire non vide, computePath renvoie une trajectoire
        //vide ce qui évite au robot de faire n'importe quoi dans ce cas.

        if (!startToObject.empty()) {
            //concaténation
            startToObject.insert(startToObject.end(), objectToGoal.begin(), objectToGoal.end());
            return startToObject;
        }
        else {
            Trajectory trajectory;
            return trajectory;
        }



    }

    else {
        //si non, on appelle directement A*
        Trajectory trajectory = Astar(Astart, Agoal);
        return trajectory;
    }
}


Trajectory Pathfinding::Astar(const PositionTrajectoire& start, const PositionTrajectoire& goal) {

    std::vector<PositionTrajectoire> chemin_solution;

    Trajectory trajectoire;
   /* if (EstUnTrajetImpossible(start, goal)) {
        return trajectoire;
    }*/
    int passageDansBoucle=0;
    PositionTrajectoire n_courant = start;
    noeud noeuddepart;
    noeuddepart.cout_g = 0;
    noeuddepart.cout_h = distance(start.x, start.y, start.orientation, goal.x, goal.y, goal.orientation);
    noeuddepart.cout_f = noeuddepart.cout_h;
    noeuddepart.position = n_courant;

    OpenSet.push_back(noeuddepart);
    n_courant = MettreAjourClosedSet();
    while (!(PosSuffisammentProches(n_courant, goal, 20.0, 0.2))&&(passageDansBoucle<1000)) {
        MettreAjourOpenSet(n_courant, goal);
        n_courant = MettreAjourClosedSet();
        if ((!isUsingSimpleTrajectory)&&(PosSuffisammentProches(n_courant, goal, 100.0, 0.4))) {
            isUsingSimpleTrajectory = true;
        }
        passageDansBoucle++;
    }

    if (passageDansBoucle == 1000) {
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


    //affichage pour debug

#ifdef ESTSURORDI
    std::cout << "chemin proposé :" << std::endl;
    for (int i=0; i<chemin_solution.size();i++) {
        std::cout << "(" << chemin_solution[i].x;
        std::cout << "," << chemin_solution[i].y;
        std::cout << ") orientation = " << chemin_solution[i].orientation;
        std::cout << " type de trajectoire : " << chemin_solution[i].trajectoire;
        std::cout << std::endl;
    }
    std::cout << "taille d'openset : " << OpenSet.size() << std::endl;
#endif
    //plus besoin d'openset et de closedset
    OpenSet.clear();
    ClosedSet.clear();

    trajectoire = positionsToTrajectory(chemin_solution);
    return (trajectoire);
}

/*bool Pathfinding::EstUnTrajetImpossible(const ObstacleMap& map, const PositionTrajectoire& start, const PositionTrajectoire& goal) {
    if (map[0].obstaclePlein) {
        return (goal.x*goal.x + goal.y*goal.y > map[1].rayon*map[1].rayon);
    }
    else if (map[1].obstaclePlein){
        return (goal.x*goal.x + goal.y*goal.y > map[0].rayon*map[0].rayon);
    }
    else {
        return true;
    }
}*/


void Pathfinding::MettreAjourOpenSet(const PositionTrajectoire& start, const PositionTrajectoire& goal) {

    // on ajoute à la liste ouverte les noeuds voisins et on calcule leurs coûts

    PositionTrajectoire tmp;
    float alpha, beta, distanceStartTmp;
    float distanceParcourue = distanceParEtape;

    if (isUsingSimpleTrajectory){
        distanceParcourue = 20.0;
    }

    for (int i=0;i<rayonsDeCourbures[i];i++) {

       // if (!isUsingSimpleTrajectory) {

        //cas où on avance de distanceParEtape suivant un rayon de courbure vers la gauche.

        alpha = distanceParcourue/(2*rayonsDeCourbures[i]);
        beta = alpha + start.orientation;
        distanceStartTmp = 2*rayonsDeCourbures[i]*cos(M_PI/2 - alpha);

        tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


        tmp.x = start.x + distanceStartTmp*cos(beta);
        tmp.y = start.y + distanceStartTmp*sin(beta);


        tmp.trajectoire = static_cast<Trajectoire>(4*i+1);


        checkCandidat(tmp, start, goal);

        //cas où on avance de distanceParcourue suivant un rayon de courbure vers la droite.

        alpha = distanceParcourue/(2*rayonsDeCourbures[i]);
        beta = start.orientation - alpha;
        distanceStartTmp = 2*rayonsDeCourbures[i]*cos(M_PI/2 - alpha);

        tmp.orientation = fmod(start.orientation - 2*alpha,2*M_PI);


        tmp.x = start.x + distanceStartTmp*cos(beta);
        tmp.y = start.y + distanceStartTmp*sin(beta);
        tmp.trajectoire = static_cast<Trajectoire>(4*i+2);

        checkCandidat(tmp, start, goal);

        //cas où on recule de distanceParcourue suivant un rayon de courbure par la gauche.

        float startOrientationRetourne;

        alpha = distanceParcourue/(2*rayonsDeCourbures[i]);
        startOrientationRetourne = start.orientation + M_PI;
        beta = startOrientationRetourne - alpha;
        distanceStartTmp = 2*rayonsDeCourbures[i]*cos(M_PI/2 - alpha);

        tmp.orientation = fmod(start.orientation - 2*alpha,2*M_PI);


        tmp.x = start.x + distanceStartTmp*cos(beta);
        tmp.y = start.y + distanceStartTmp*sin(beta);

        tmp.trajectoire = static_cast<Trajectoire>(4*i+3);

        checkCandidat(tmp, start, goal);

        //cas où on recule de distanceParcourue suivant un rayon de courbure par la droite.

        alpha = distanceParcourue/(2*rayonsDeCourbures[i]);
        startOrientationRetourne = start.orientation + M_PI;
        beta = startOrientationRetourne + alpha;
        distanceStartTmp = 2*rayonsDeCourbures[i]*cos(M_PI/2 - alpha);

        tmp.orientation = fmod(start.orientation + 2*alpha,2*M_PI);


        tmp.x = start.x + distanceStartTmp*cos(beta);
        tmp.y = start.y + distanceStartTmp*sin(beta);

        tmp.trajectoire = static_cast<Trajectoire>(4*i+4);

        checkCandidat(tmp, start, goal);
      //  }
    }
    //cas ou on avance en ligne droite de distanceParEtape

    tmp.x = start.x + distanceParcourue*cos(start.orientation);
    tmp.y = start.y + distanceParcourue*sin(start.orientation);
    tmp.orientation = start.orientation;
    tmp.trajectoire = AvancerToutDroit;

    checkCandidat(tmp, start, goal);

    //cas ou on recule en ligne droite de distanceParcourue

    tmp.x = start.x - distanceParcourue*cos(start.orientation);
    tmp.y = start.y - distanceParcourue*sin(start.orientation);
    tmp.orientation = start.orientation;
    tmp.trajectoire = ReculerToutDroit;

    checkCandidat(tmp, start, goal);

    if (rotationAllowed) {
        /*
        //cas où on tourne de distanceParEtape/R3 dans le sens trigonométrique.

        tmp.orientation = start.orientation + 0.2;
        tmp.x = start.x;
        tmp.y = start.y;
        tmp.trajectoire = TournerSurPlaceTrigo;


        checkCandidat(tmp, start, goal);*/

        //cas où on tourne sur place dans le sens horaire.

        tmp.orientation = start.orientation - 0.2;
        tmp.x = start.x;
        tmp.y = start.y;
        tmp.trajectoire = TournerSurPlaceAntiTrigo;


        checkCandidat(tmp, start, goal);
    }

}

void Pathfinding::checkCandidat(const PositionTrajectoire& candidat, const PositionTrajectoire& start, const PositionTrajectoire& goal) {

    //on vérifie la présence d'Obstacle

    if (estSurUnObstacle(candidat.x,candidat.y) == -1) {
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



Pathfinding::PositionTrajectoire Pathfinding::MettreAjourClosedSet() {
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
    return (min_noeud.position);
}

int Pathfinding::estSurUnObstacle(float x, float y) {

    for (int i=0;i<obstaclesSurLaMap.size();i++) {

        //si l'obstacle est à pousser, il ne faut pas l'éviter
        if (obstaclesSurLaMap[i].isPushed()) {
            continue;
        }

        //si l'obstacle est un CERCLE
        if (obstaclesSurLaMap[i].getShape()==0) {
            Position positionObstacle;
            obstaclesSurLaMap[i].getCenter(positionObstacle);
            if (positionObstacle.orientation == 0){
                if (pow(x-positionObstacle.x,2)+pow(y-positionObstacle.y,2) < pow(obstaclesSurLaMap[i].getRadius(),2)) {
                    return i;
                }
            }
            else {
                //a faire
            }
        }

        //si l'obstacle est un RECTANGLE
        else if (obstaclesSurLaMap[i].getShape()==1) {
            Position positionObstacle;
            obstaclesSurLaMap[i].getCenter(positionObstacle);
            if (positionObstacle.orientation == 0) {
                if ( (x < positionObstacle.x + obstaclesSurLaMap[i].getXRadius()) &&
                         (x > positionObstacle.x - obstaclesSurLaMap[i].getXRadius()) &&
                         (y < positionObstacle.y + obstaclesSurLaMap[i].getYRadius()) &&
                         (y > positionObstacle.y - obstaclesSurLaMap[i].getYRadius()) ) {
                    return i;
                }
                else {
                    //a faire
                }
            }
        }

        //si l'obstacle est un BORD_DE_TABLE
        else if (obstaclesSurLaMap[i].getShape()==2) {
            Position positionObstacle;
            obstaclesSurLaMap[i].getCenter(positionObstacle);
            if (positionObstacle.orientation == 0){
                if (!( (x < positionObstacle.x + obstaclesSurLaMap[i].getXRadius()) &&
                       (x > positionObstacle.x - obstaclesSurLaMap[i].getXRadius()) &&
                       (y < positionObstacle.y + obstaclesSurLaMap[i].getYRadius()) &&
                       (y > positionObstacle.y - obstaclesSurLaMap[i].getYRadius()) )) {

                    return i;
                }
            }

            else {
                //a faire
            }

        }

        //si l'obstacle n'a pas été précisé (problème en amont)
        else {
            //std::cerr << "problèmes dans l'obstaclemap : getShape renvoie un nombre > 2";
        }
    }
    return -1;
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


//stopAfterMove :
Trajectory Pathfinding::positionsToTrajectory(const std::vector<PositionTrajectoire>& chemin_solution) {

    Trajectory trajectoire;
    float pasPourTourner = 0.2;

    for (int i=0;i<chemin_solution.size();i++) {
        UnitMove unitmove;
        unitmove.stopAfterMove = true;


        switch (chemin_solution[i].trajectoire) {

            case Depart:
                continue;
                break;
            case Avancer1g:
                unitmove.setLengthMm(distanceParEtape);
                unitmove.setBendRadiusMm(rayonsDeCourbures[0]);
                unitmove.setSpeedMm_S(400);
                break;

            case Avancer1d:
                unitmove.setLengthMm(distanceParEtape);
                unitmove.setBendRadiusMm(-1*rayonsDeCourbures[0]);
                unitmove.setSpeedMm_S(400);
                break;

            case Reculer1g:
                unitmove.setLengthMm(-1*distanceParEtape);
                unitmove.setBendRadiusMm(rayonsDeCourbures[0]);
                unitmove.setSpeedMm_S(400);
                break;

            case Reculer1d:
                unitmove.setLengthMm(-1*distanceParEtape);
                unitmove.setBendRadiusMm(-1*rayonsDeCourbures[0]);
                unitmove.setSpeedMm_S(400);
                break;

            case Avancer2g:
                unitmove.setLengthMm(distanceParEtape);
                unitmove.setBendRadiusMm(rayonsDeCourbures[1]);
                unitmove.setSpeedMm_S(400);
                break;

            case Avancer2d:
                unitmove.setLengthMm(distanceParEtape);
                unitmove.setBendRadiusMm(-1*rayonsDeCourbures[1]);
                unitmove.setSpeedMm_S(400);
                break;

            case Reculer2g:
                unitmove.setLengthMm(-1*distanceParEtape);
                unitmove.setBendRadiusMm(rayonsDeCourbures[1]);
                unitmove.setSpeedMm_S(400);
                break;

            case Reculer2d:
                unitmove.setLengthMm(-1*distanceParEtape);
                unitmove.setBendRadiusMm(-1*rayonsDeCourbures[1]);
                unitmove.setSpeedMm_S(400);
                break;

            case Avancer3g:
                unitmove.setLengthMm(distanceParEtape);
                unitmove.setBendRadiusMm(rayonsDeCourbures[2]);
                unitmove.setSpeedMm_S(400);
                break;

            case Avancer3d:
                unitmove.setLengthMm(distanceParEtape);
                unitmove.setBendRadiusMm(-1*rayonsDeCourbures[2]);
                unitmove.setSpeedMm_S(400);
                break;

            case Reculer3g:
                unitmove.setLengthMm(-1*distanceParEtape);
                unitmove.setBendRadiusMm(rayonsDeCourbures[2]);
                unitmove.setSpeedMm_S(400);
                break;

            case Reculer3d:
                unitmove.setLengthMm(-1*distanceParEtape);
                unitmove.setBendRadiusMm(-1*rayonsDeCourbures[2]);
                unitmove.setSpeedMm_S(400);
                break;

            case AvancerToutDroit:
                unitmove.setLengthMm(distanceParEtape);
                unitmove.setBendRadiusMm(INFINITE_RADIUS);
                unitmove.setSpeedMm_S(450);
                break;

            case ReculerToutDroit:
                unitmove.setLengthMm(-1*distanceParEtape);
                unitmove.setBendRadiusMm(INFINITE_RADIUS);
                unitmove.setSpeedMm_S(450);
                break;

            case TournerSurPlaceTrigo:
                unitmove.setLengthRadians(pasPourTourner);
                unitmove.setBendRadiusMm(0);
                unitmove.setSpeedMm_S(400);
                break;

            case TournerSurPlaceAntiTrigo:
                unitmove.setLengthRadians(-1*pasPourTourner);
                unitmove.setBendRadiusMm(0);
                unitmove.setSpeedMm_S(400);
                break;

        }


        trajectoire.insert(trajectoire.begin(), unitmove);
    }

    //concaténation
    int i = 0;
    while (i<trajectoire.size()-1) {
        if ((trajectoire[i].getLengthMm()*trajectoire[i+1].getLengthMm() > 0)&&
               (trajectoire[i].getBendRadiusMm() == trajectoire[i+1].getBendRadiusMm())) {
           if (trajectoire[i].getBendRadiusMm() == 0) {
               trajectoire[i].setLengthRadians(trajectoire[i].getLengthRadians() + trajectoire[i+1].getLengthRadians());
           }
           else {
               trajectoire[i].setLengthMm(trajectoire[i].getLengthMm() + trajectoire[i+1].getLengthMm());
           }
           trajectoire.erase(trajectoire.begin()+i+1);
        }

        else {
            i++;
        }
    }

    //décider de la variable stopAfterMove
    for (int i=0;i<trajectoire.size();i++) {
        float R1,R2;
        R1 = trajectoire[i].getBendRadiusMm();
        if (i!= trajectoire.size()-1) {
            R2 = trajectoire[i+1].getBendRadiusMm();
            if ((R2==0)||(R1==0)) {
                trajectoire[i].stopAfterMove = true;
            }
            else {
                trajectoire[i].stopAfterMove = ((R1-R2 > SEUIL_ROTATION)||(R1-R2 < -1*SEUIL_ROTATION));
            }
        }
        else {
            trajectoire[i].stopAfterMove = true;
        }
    }
    return trajectoire;
}

float Pathfinding::distance(float x1, float y1, float o1, float x2, float y2, float o2){
    return ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (o1-o2)*(o1-o2)*coeffOrientation);
}


bool Pathfinding::PosEgales(const PositionTrajectoire& p1, const PositionTrajectoire& p2) {
    return (
            (p1.orientation == p2.orientation) &&
            (p1.x == p2.x) &&
            (p1.y == p2.y)
           );
}

bool Pathfinding::PosSuffisammentProches(const PositionTrajectoire& p1, const PositionTrajectoire& p2, float precisionXY, float precisionOrientation) {
    return (
            (p1.orientation < p2.orientation + precisionOrientation) &&
            (p2.orientation - precisionOrientation < p1.orientation) &&

            (p1.x < p2.x + precisionXY) &&
            (p2.x - precisionXY < p1.x) &&

            (p1.y < p2.y + precisionXY) &&
            (p2.y - precisionXY < p1.y)
           );
}

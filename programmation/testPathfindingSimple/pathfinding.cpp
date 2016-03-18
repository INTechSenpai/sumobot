#include "pathfinding.h"
#include <iostream>
#include <math.h>
Pathfinding::Pathfinding()
{}

Trajectory Pathfinding::computePath(Position start, Position pointIntermediaire, Position goal) {
    float xCentre, yCentre, rayonCourbure, distanceParcourue;
    Trajectory trajectoire;
    xCentre = -1*(
                (pointIntermediaire.y*(goal.y*goal.y - start.y*start.y + goal.x*goal.x - start.x*start.x ) +
                 start.y*(-1*goal.y*goal.y - goal.x*goal.x + pointIntermediaire.x*pointIntermediaire.x ) +
                 start.y*start.y*goal.y - pointIntermediaire.x*pointIntermediaire.x*goal.y +
                 start.x*start.x*goal.y + pointIntermediaire.y*pointIntermediaire.y*(start.y - goal.y) )

            / (2*pointIntermediaire.x*goal.y - 2*start.x*goal.y + (2*start.x - 2*goal.x)*pointIntermediaire.y +
               (2*goal.x - 2*pointIntermediaire.x )*start.y)
                );

    yCentre = (pointIntermediaire.x*(goal.y*goal.y + goal.x*goal.x - start.x*start.x ) +
               start.x*(-1*goal.y*goal.y - goal.x*goal.x ) +
               (start.x - goal.x)*pointIntermediaire.y*pointIntermediaire.y + (goal.x - pointIntermediaire.x)*start.y*start.y +
               start.x*start.x*goal.x + pointIntermediaire.x*pointIntermediaire.x*(start.x - goal.x ))
               /
              (2*pointIntermediaire.x*goal.y - 2*start.x*goal.y + (2*start.x - 2*goal.x )*pointIntermediaire.y +
              (2*goal.x - 2*pointIntermediaire.x)*start.y);

    rayonCourbure = sqrt((start.x - xCentre)*(start.x - xCentre) + (start.y - yCentre)*(start.y - yCentre));

    float valeurAcos1 = (start.x - xCentre)/rayonCourbure;
    float valeurAcos2 = (goal.x - xCentre)/rayonCourbure;

    if ( (valeurAcos1<-1)||(valeurAcos1>1)||(valeurAcos2<-1)||(valeurAcos2>1)) {
        std::cout << "mauvais paramètres !" << std::endl;
        return trajectoire;
    }
    distanceParcourue = rayonCourbure*(acos(valeurAcos1)-acos(valeurAcos2));
    if (distanceParcourue < 0) {
        distanceParcourue *= -1;
    }

    std::cout << " xCentre : " << xCentre;
    std::cout << " yCentre : " << yCentre;
    std::cout << std::endl;
    std::cout << " distanceParcourue : " << distanceParcourue;
    std::cout << std::endl;

    std::cout << " rayonCourbure : "<< rayonCourbure << std::endl;

    UnitMove unitmove;
    unitmove.setBendRadiusMm(rayonCourbure);
    unitmove.setLengthMm(distanceParcourue);
    unitmove.setSpeedMm_S(sqrt(start.xSpeed*start.xSpeed + start.ySpeed*start.ySpeed));
    trajectoire.push_back(unitmove);
    return trajectoire;

}

Trajectory Pathfinding::computePath(Position start, Position goal) {
    Trajectory trajectoire;
    UnitMove unitmove;
    unitmove.setSpeedMm_S(sqrt(start.xSpeed*start.xSpeed + start.ySpeed*start.ySpeed));
    start.orientation = fmod(start.orientation,2*M_PI);
    goal.orientation = fmod(goal.orientation,2*M_PI);

    //rotation pure
    if ((start.x == goal.x)&&(start.y==goal.y)) {
        unitmove.stopAfterMove = true;
        unitmove.setBendRadiusMm(0);
        unitmove.setLengthRadians(fmod(start.orientation - goal.orientation,2*M_PI));
    }
    //trajectoire droite
    else if ((start.orientation == goal.orientation)) {
        unitmove.stopAfterMove = false;
        unitmove.setBendRadiusMm(INFINITE_RADIUS);
        unitmove.setLengthMm(sqrt((start.x - goal.x)*(start.x - goal.x) + (start.y - goal.y)*(start.y - goal.y)));
    }
    //trajectoire courbe
    else {
        float rayonCourbure = sqrt((start.x - goal.x)*(start.x - goal.x) + (start.y - goal.y)*(start.y - goal.y)) /
                              (2*cos( (M_PI/2) - fmod(start.orientation - goal.orientation,2*M_PI) / 2));
        float longueurMvt = rayonCourbure*(start.orientation - goal.orientation);
       /* if (longueurMvt < 0) {
            longueurMvt *= -1;
        }*/

        unitmove.stopAfterMove = false;
        unitmove.setBendRadiusMm(rayonCourbure);
        unitmove.setLengthMm(longueurMvt);
    }

    trajectoire.push_back(unitmove);
    std::cout << "R : " << unitmove.getBendRadiusMm() << std::endl;
    std::cout << "L : " << unitmove.getLengthMm() << std::endl;
    return trajectoire;

}

/*
Trajectory Pathfinding::computePath(float rot, float rayonCourbure, float longueur) {

    Trajectory trajectoire;
    UnitMove rotation;
    UnitMove ligneCourbe;

    rotation.stopAfterMove = true;
    rotation.setBendRadiusMm(0);
    rotation.setLengthRadians(fmod(rot,2*M_PI));
    rotation.setSpeedMm_S(sqrt(start.xSpeed*start.xSpeed + start.ySpeed*start.ySpeed));

    trajectoire.push_back(rotation);

    ligneCourbe.stopAfterMove = false;
    ligneCourbe.setBendRadiusMm(rayonCourbure);
    ligneCourbe.setLengthMm(longueur);
    ligneCourbe.setSpeedMm_S(sqrt(start.xSpeed*start.xSpeed + start.ySpeed*start.ySpeed));

    trajectoire.push_back(ligneCourbe);

    return trajectoire;

}*/

Trajectory computePathFoncerRobot(Position start, Position goal) {

    Trajectory trajectoire;

    if (!((start.orientation - goal.orientation < 0.14)&&(-0.14 < start.orientation - goal.orientation))) {
        UnitMove rotation;
        rotation.stopAfterMove = true;
        rotation.setBendRadiusMm(0);
        rotation.setLengthRadians(fmod(start.orientation - goal.orientation,2*M_PI));
        rotation.setSpeedMm_S(sqrt(start.xSpeed*start.xSpeed + start.ySpeed*start.ySpeed));

        trajectoire.push_back(rotation);

    }


    UnitMove avancerDroit;
    avancerDroit.stopAfterMove = false;
    avancerDroit.setBendRadiusMm(INFINITE_RADIUS);
    avancerDroit.setLengthMm(sqrt((start.x - goal.x)*(start.x - goal.x) + (start.y - goal.y)*(start.y - goal.y)));
    avancerDroit.setSpeedMm_S(sqrt(start.xSpeed*start.xSpeed + start.ySpeed*start.ySpeed));

    trajectoire.push_back(avancerDroit);

    return trajectoire;
}

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

    std::cout << "valeurAcos1 : " << valeurAcos1 << std::endl;
    std::cout << "valeurAcos2 : " << valeurAcos2 << std::endl;

    if ( (valeurAcos1<-1)||(valeurAcos1>1)||(valeurAcos2<-1)||(valeurAcos2>1)) {
        return trajectoire;
    }
    distanceParcourue = rayonCourbure*(acos(valeurAcos1)-acos(valeurAcos2));

    std::cout << "rayonCourbure : " << rayonCourbure << std::endl;
    std::cout << "distanceParcourue : " << distanceParcourue << std::endl;

    if (distanceParcourue < 0) {
        distanceParcourue *= -1;
    }

    UnitMove unitmove;
    unitmove.setBendRadiusMm(rayonCourbure);
    unitmove.setLengthMm(distanceParcourue);
    unitmove.setSpeedMm_S(350);
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

Trajectory Pathfinding::computePathFoncerRobot(Position start, Position goal, float longueur) {

    Trajectory trajectoire;

    if ((fmod(goal.orientation - start.orientation,2*M_PI) > M_PI - 0.14)&&
        (fmod(goal.orientation - start.orientation,2*M_PI) < M_PI + 0.14)) {
        longueur *=-1;
    }

    float angleRotation = goal.orientation - start.orientation;

    while (angleRotation > M_PI/2) {
        angleRotation = angleRotation - M_PI;
    }
    while (angleRotation < -1*M_PI/2) {
        angleRotation = M_PI + angleRotation;
    }


    std::cout << "angleRotation : " << angleRotation << std::endl;
    std::cout << "longueur : " << longueur << std::endl;

    if (! ((angleRotation < 0.14)&&(-0.14 < angleRotation))) {

        UnitMove rotation;
        rotation.stopAfterMove = true;
        rotation.setBendRadiusMm(0);
        rotation.setSpeedMm_S(sqrt(start.xSpeed*start.xSpeed + start.ySpeed*start.ySpeed));

        rotation.setLengthRadians(angleRotation);


        trajectoire.push_back(rotation);

    }


    else {
    UnitMove avancerDroit;
    avancerDroit.stopAfterMove = false;
    avancerDroit.setBendRadiusMm(INFINITE_RADIUS);


    avancerDroit.setLengthMm(longueur);
    avancerDroit.setSpeedMm_S(sqrt(start.xSpeed*start.xSpeed + start.ySpeed*start.ySpeed));

    trajectoire.push_back(avancerDroit);
    }

    return trajectoire;
}

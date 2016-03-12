#include "pathfinding.h"
#include <iostream>
#include <math.h>
Pathfinding::Pathfinding()
{}

Trajectory Pathfinding::computePath(Position start, Position pointIntermediaire, Position goal, ObstacleMap map) {
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

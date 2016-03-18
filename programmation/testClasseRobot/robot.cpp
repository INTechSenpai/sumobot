#include "robot.h"

Robot::Robot(){}


void Robot::strategy() {

    //met à jour les obstacles sur la map

    Position positionRobot = motionControlSystem.getPosition();

    RelativeObstacleMap obstacles;
    SensorMgr.getRelativeObstacleMap(obstacles); //obstacle map relative

    Table table(50,400,0,0);

    bool estPerdu = table.updateObstacleMap(obstacles, positionRobot); //obstacle map absolue + correction trajectoire (modifie variableJoueur)

    motionControlSystem.setPosition(positionRobot);

    //Si on est perdu, on considere qu'on est au milieu et on fonce sur l'ennemi
    if (estPerdu) {
        Position goal = table.getRobotAdverse().position;
        goal.orientation = table.getAngleAbsoluRA();
        Trajectory trajectoireFoncerRobot = pathfinding.computePathFoncerRobot(positionRobot, goal);
        motionControlSystem.setTrajectory(trajectoireFoncerRobot);
    }

    //sinon....
    else {

        Position positionAdverse = table.getRobotAdverse().position;
        //proposition 2 : se diriger en fonction de 3 points sur le coté de l'ennemi dans le cas général,
        //foncer sur le robot ennemi quand on est trop pres/le robot adverse au bord de la table

        //si le robot ennemi n'est pas détecté.
        if ((positionAdverse.x == 1000)&&(positionAdverse.y == 1000)) {

            Position goal = positionRobot;
            goal.orientation = positionRobot.orientation + 2*M_PI - 0.1;
            Trajectory trajectoireTourner = pathfinding.computePath(positionRobot, goal);
            if (!trajectoireTourner.empty()){
                motionControlSystem.setTrajectory(trajectoireTourner);
            }

        }

        else {

            //tourner vers le robot ennemi, en verifiant qu'on ne dépasse pas sur la table.
            Position pointIntermediaire, goal;

            //position d'arrivée : point derriere le robot adverse
            goal.x = positionAdverse.x + table.getDistanceAway()*cos(table.getAngleAbsoluRA() - (M_PI/4));
            goal.y = positionAdverse.y + table.getDistanceAway()*sin(table.getAngleAbsoluRA() - (M_PI/4));

            //point intermediaire pour passer a coté du robot adverse
            pointIntermediaire.x = positionAdverse.x + (table.getDistanceAway()/2)*cos(table.getAngleAbsoluRA() - (2*M_PI/3));
            pointIntermediaire.y = positionAdverse.y + (table.getDistanceAway()/2)*sin(table.getAngleAbsoluRA() - (2*M_PI/3));

            //todo : verification de la

            //creation de la trajectoire
            Trajectory trajectoireContourner = pathfinding.computePath(positionRobot, pointIntermediaire, goal);

            if (!trajectoireContourner.empty()){
                motionControlSystem.setTrajectory(trajectoireContourner);
            }
        }
    }


}

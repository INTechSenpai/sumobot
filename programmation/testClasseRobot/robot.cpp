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
    //verifie si une trajectoire de rotation n'a pas deja lieu
    if (motionControlSystem.getCurrentMove()>=1) {

        //Si on est perdu, on considere qu'on est au milieu et on fonce sur l'ennemi
        if (estPerdu) {
            Position goal = table.getRobotAdverse().position;
            goal.orientation = table.getAngleAbsoluRA();
            Trajectory trajectoireFoncerRobot = pathfinding.computePathFoncerRobot(positionRobot, goal);
            motionControlSystem.setTrajectory(trajectoireFoncerRobot);
        }

        //sinon....
        else {
            /* proposition 1 (malcolm) :
            Trajectory trajectoire = pathfinding.computePath(table.getAngleAbsoluRA(), table.getDistanceAway(), 150);
            motionControlSystem.setTrajectory(trajectoire);
            */

            Position goal = table.getRobotAdverse().position;
            //proposition 2 : se diriger en fonction de 3 points sur le coté de l'ennemi dans le cas général,
            //foncer sur le robot ennemi en cas favorable

            //si le robot ennemi est sur le coté, on lui fonce dessus.
            if (((goal.orientation + (M_PI/2) < positionRobot.orientation + 0.3)&&
                (goal.orientation + (M_PI/2) > positionRobot.orientation - 0.3)) ||

               ((goal.orientation - (M_PI/2) < positionRobot.orientation + 0.3)&&
                (goal.orientation - (M_PI/2) > positionRobot.orientation - 0.3)))

            {

                Position goal = table.getRobotAdverse().position;
                goal.orientation = table.getAngleAbsoluRA();
                Trajectory trajectoireFoncerRobot = pathfinding.computePathFoncerRobot(positionRobot, goal);
                motionControlSystem.setTrajectory(trajectoireFoncerRobot);

            }

            else {

                //tourner vers le robot ennemi vers la gauche, en verifiant qu'on ne dépasse pas sur la table.
                Position pointIntermediaire, goal;
                //pointIntermediaire et goal à definir
                Trajectory trajectoireContourner = pathfinding.computePath(positionRobot, pointIntermediaire, goal);
                motionControlSystem.setTrajectory(trajectoireContourner);
            }
        }
    }

}

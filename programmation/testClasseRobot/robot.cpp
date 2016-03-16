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
    if (motionControlSystem.getCurrentMove()==1) {
        //Si on est perdu, on considere qu'on est au milieu et on fonce sur l'ennemi
        if (estPerdu) {
            Position goal = table.getRobotAdverse().position;
            goal.orientation = table.getAngleAbsoluRA();
            Trajectory trajectoireFoncerRobot = pathfinding.computePathFoncerRobot(positionRobot, goal);
            motionControlSystem.setTrajectory(trajectoireFoncerRobot);
        }

        //sinon....
        else {
            Trajectory trajectoire = pathfinding.computePath(table.getAngleAbsoluRA(), table.getDistanceAway(), 150);
            motionControlSystem.setTrajectory(trajectoire);

        }
    }

}

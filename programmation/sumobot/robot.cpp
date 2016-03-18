#include "robot.h"
#include "MotionControlSystem.h"

Robot::Robot(){}


void Robot::strategy(Table & table, bool estPerdu, Position & positionRobot, Trajectory & trajectoireRetour) 
{
    //Si on est perdu, on considere qu'on est au milieu et on fonce sur l'ennemi
    if (estPerdu) 
	{
        Position goal = table.getRobotAdverse().position;
        goal.orientation = table.getAngleAbsoluRA();

		trajectoireRetour = pathfinding.computePathFoncerRobot(positionRobot, goal, table.getDistanceAway());
		return;
    }

    //sinon....
    else 
	{

        Position positionAdverse = table.getRobotAdverse().position;
        //proposition 2 : se diriger en fonction de 3 points sur le coté de l'ennemi dans le cas général,
        //foncer sur le robot ennemi quand on est trop pres/le robot adverse au bord de la table

        //si le robot ennemi n'est pas détecté.
        if ((positionAdverse.x == 1000)&&(positionAdverse.y == 1000)) 
		{

            Position goal = positionRobot;
			if (!motionControlSystem.isMoving())
			{
				goal.orientation = positionRobot.orientation + 2 * M_PI - 0.1;
				trajectoireRetour = pathfinding.computePath(positionRobot, goal);
				return;
			}
			else
				return;

        }

        else 
		{

            //tourner vers le robot ennemi, en verifiant qu'on ne dépasse pas sur la table.
            Position pointIntermediaire, goal;

            //position d'arrivée : point derriere le robot adverse
            goal.x = positionAdverse.x + table.getRobotAdverse().rayon*cos(table.getAngleAbsoluRA() - (M_PI/4));
            goal.y = positionAdverse.y + table.getRobotAdverse().rayon*sin(table.getAngleAbsoluRA() - (M_PI/4));

            //point intermediaire pour passer a coté du robot adverse
            pointIntermediaire.x = positionAdverse.x + (table.getDistanceAway()/2)*cos(table.getAngleAbsoluRA() - (2*M_PI/3));
            pointIntermediaire.y = positionAdverse.y + (table.getDistanceAway()/2)*sin(table.getAngleAbsoluRA() - (2*M_PI/3));

            //si la trajectoire courbe sort de la table, on fait une ligne droite
            if (goal.x*goal.x + goal.y*goal.y > table.getBordDeTable().rayon*table.getBordDeTable().rayon) 
			{
				trajectoireRetour = pathfinding.computePathFoncerRobot(positionRobot, goal, table.getDistanceAway());
				return;
            }
            else 
			{
                //creation de la trajectoire courbe
				trajectoireRetour = pathfinding.computePath(positionRobot, pointIntermediaire, goal);
				return;
            }
        }
    }
}

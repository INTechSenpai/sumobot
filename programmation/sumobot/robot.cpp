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
        //si on est trop proche du bord de table, on fonce sur l'ennemi
        if (positionRobot.x*positionRobot.x + positionRobot.y*positionRobot.y > 
            (table.getBordDeTable.rayon - 40)*(table.getBordDeTable.rayon - 40)) {

            Position goal = table.getRobotAdverse().position;
            goal.orientation = table.getAngleAbsoluRA();
            trajectoireRetour = pathfinding.computePathFoncerRobot(positionRobot, goal, table.getDistanceAway());
            return;

        }
        Position positionAdverse = table.getRobotAdverse().position;

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

        //si le robot ennemi est trop proche, on lui fonce dessus
        else if (table.getDistanceAway() < 150)
        {
            Position goal = table.getRobotAdverse().position;
            goal.orientation = table.getAngleAbsoluRA();
            trajectoireRetour = pathfinding.computePathFoncerRobot(positionRobot, goal, table.getDistanceAway());
            return;
        }
        //sinon le robot prend une trajectoire courbe pour passer derriere l'ennemi
        else
		{

            float rotation = table.getAngleAbsoluRA() - M_PI/3 - positionRobot.orientation;
            float rayonCourbure = table.getDistanceAway()*1.14;
            float longeur = M_PI * table.getDistanceAway()/2;

            if ((rotation > 0.19)||(rotation < -0.19))
            {
                Position goal = positionRobot;
                goal.orientation = positionRobot.orientation + rotation;
                trajectoireRetour = pathfinding.computePath(positionRobot, goal);
            }
            else 
            {
                Trajectory trajectoireRetour = pathfinding.computePath(rayonCourbure, longueur);
            }

            

            /*
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
            */
        }
    }
}

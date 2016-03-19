#include "robot.h"
#include "MotionControlSystem.h"

Robot::Robot(){}


void Robot::strategy(Table & table, bool estPerdu, bool isMoving, Position & positionRobot, Trajectory & trajectoireRetour) 
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
            (table.getBordDeTable().rayon - 40)*(table.getBordDeTable().rayon - 40)) {

            Position goal = table.getRobotAdverse().position;
            goal.orientation = table.getAngleAbsoluRA();
            trajectoireRetour = pathfinding.computePathFoncerRobot(positionRobot, goal, table.getDistanceAway());
            return;

        }
        Position positionAdverse = table.getRobotAdverse().position;

        //si le robot ennemi n'est pas détecté.
        /*if ((positionAdverse.x == 1000)&&(positionAdverse.y == 1000)) 
		{

			if (!isMoving)
			{
				Position goal = positionRobot;
				goal.orientation = positionRobot.orientation + 2 * M_PI - 0.1;
				trajectoireRetour = pathfinding.computePath(positionRobot, goal);
				return;
			}

        }

        //si le robot ennemi est trop proche, on lui fonce dessus
        else*/ if (table.getDistanceAway() < 150)
        {
            Position goal = table.getRobotAdverse().position;
            goal.orientation = table.getAngleAbsoluRA();
            trajectoireRetour = pathfinding.computePathFoncerRobot(positionRobot, goal, table.getDistanceAway());
            return;
        }
        //sinon le robot prend une trajectoire courbe pour passer derriere l'ennemi
        else
		{
			float rotation = table.getAngleAbsoluRA() + M_PI/3 - positionRobot.orientation;
            int rayonCourbure = (int)table.getDistanceAway()/2;
            float longueur = M_PI * table.getDistanceAway()/2;
			if (!((positionAdverse.x == 1000) && (positionAdverse.y == 1000)))
				ancientRayon = rayonCourbure;

			while (rotation > M_PI / 2) {
				rotation = rotation - M_PI;
			}
			while (rotation < -1 * M_PI / 2) {
				rotation = M_PI + rotation;
			}

            if ((rotation > 0.19)||(rotation < -0.19))
            {
                Position goal = positionRobot;
                goal.orientation = positionRobot.orientation + rotation;
                trajectoireRetour = pathfinding.computePath(positionRobot, goal);
				//Trajectory trajectoireAvancer = pathfinding.computePath(rayonCourbure, longueur);
				//UnitMove avancerCourbe = trajectoireAvancer.front();
				//trajectoireRetour.push_back(avancerCourbe);
				//trajectoireRetour = pathfinding.computePath(rayonCourbure, longueur);
				if ((positionAdverse.x == 1000) && (positionAdverse.y == 1000))
				trajectoireRetour = pathfinding.computePath(ancientRayon, 500);
				return;
            }
            else 
            {
                trajectoireRetour = pathfinding.computePath(rayonCourbure, longueur);
				//trajectoireRetour = pathfinding.computePath(INFINITE_RADIUS, longueur);
				return;
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

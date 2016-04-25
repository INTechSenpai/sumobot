#include "Table.h"

Table::Table()
{
}

ObstacleMap Table::getObstacleMap() const
{
	return obstacleMap;
}

void Table::initObstacleMap(Side side)
{
	obstacleMap.fixedInvisible.clear();
	obstacleMap.fixedVisible.clear();
	obstacleMap.movableInvisible.clear();
	obstacleMap.movableVisible.clear();
	obstacleMap.oponentRobot.clear();
	obstacleMap.toBeSpecified.clear();

	Position centre;
	Obstacle cercle(centre, CIRCLE);
	Obstacle rectangle(centre, RECTANGLE);
	Obstacle edgeOfTable(centre, EDGE_OF_TABLE);


	/*
		##### Bords de table ##### 
	*/
	// Bord principal
	centre.x = 0;
	centre.y = 1000;
	edgeOfTable.setCenter(centre);
	edgeOfTable.setXRadius(1500);
	edgeOfTable.setYRadius(1000);
	obstacleMap.fixedVisible.push_back(edgeOfTable);

	// Support à coquillages n°1
	centre.x = 1500;
	centre.y = 0;
	cercle.setCenter(centre);
	cercle.setRadius(250);
	obstacleMap.fixedVisible.push_back(cercle);

	// Support à coquillages n°2
	centre.x = -1500;
	centre.y = 0;
	cercle.setCenter(centre);
	cercle.setRadius(250);
	obstacleMap.fixedVisible.push_back(cercle);



	/*
		##### Eléments de décors fixe #####
	*/
	
	// Tasseau de limite de la dune, côté x>0
	centre.x = 689;
	centre.y = 1900;
	rectangle.setCenter(centre);
	rectangle.setXRadius(11);
	rectangle.setYRadius(100);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Tasseau de limite de la dune, côté x<0
	centre.x = -689;
	centre.y = 1900;
	rectangle.setCenter(centre);
	rectangle.setXRadius(11);
	rectangle.setYRadius(100);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Tasseau de limite de NOTRE bac à sable
	if(side == GREEN)
		centre.x = 312;
	else
		centre.x = -312;
	centre.y = 761;
	rectangle.setCenter(centre);
	rectangle.setXRadius(288);
	rectangle.setYRadius(11);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Bac à sable adverse (inclu le tasseau de délimitation)
	if(side == GREEN)
		centre.x = -312;
	else
		centre.x = 312;
	centre.y = 950;
	rectangle.setCenter(centre);
	rectangle.setXRadius(288);
	rectangle.setYRadius(300);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Vitre centrale 
	centre.x = 0;
	centre.y = 950;
	rectangle.setCenter(centre);
	rectangle.setXRadius(24);
	rectangle.setYRadius(300);
	obstacleMap.fixedVisible.push_back(rectangle);

	// Serivette adverse
	if (side == GREEN)
		centre.x = -1350;
	else
		centre.x = 1350;
	centre.y = 1150;
	rectangle.setCenter(centre);
	rectangle.setXRadius(150);
	rectangle.setYRadius(250);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Dune de sable (simplifiée)
	centre.x = 0;
	centre.y = 1940;
	rectangle.setCenter(centre);
	rectangle.setXRadius(678);
	rectangle.setYRadius(60);
	obstacleMap.fixedInvisible.push_back(rectangle);


	/*
		##### Eléments de jeu #####
	*/

	// Tas de sable face à notre serviette
	if (side == GREEN)
		centre.x = 850;
	else
		centre.x = -850;
	centre.y = 1100;
	rectangle.setCenter(centre);
	rectangle.setXRadius(60);
	rectangle.setYRadius(60);
	if (side == GREEN)
		rectangle.setOrientationToPush(PI);
	else
		rectangle.setOrientationToPush(0);
	rectangle.setPriority(KNOWN_SAND_PRIORITY);
	obstacleMap.movableVisible.push_back(rectangle);



	/*
	################################
		   ## Coquillages ##
	################################
	*/
	
	// Gestion de la symétrie
	int mirror;
	if (side == GREEN)
		mirror = 1;
	else
		mirror = -1;

	// De notre côté
	centre.x = mirror * 1300;
	centre.y = 750;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 1300;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 800;
	centre.y = 750;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 3 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 800;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * 800;
	centre.y = 150;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 600;
	centre.y = 550;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 2 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * 300;
	centre.y = 350;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	// Au centre
	centre.x = 0;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 2 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = 0;
	centre.y = 150;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 2 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	// Côté adverse
	centre.x = mirror * -1300;
	centre.y = 750;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * -1300;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * -800;
	centre.y = 750;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * -800;
	centre.y = 450;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 3 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * -800;
	centre.y = 150;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	cercle.setOrientationToPush(ORIENTATION_BACKWARD);
	cercle.setPriority(SHELL_PRIORITY * 1 / 5);
	obstacleMap.movableInvisible.push_back(cercle);

	centre.x = mirror * -600;
	centre.y = 550;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

	centre.x = mirror * -300;
	centre.y = 350;
	cercle.setCenter(centre);
	cercle.setRadius(40);
	obstacleMap.fixedInvisible.push_back(cercle);

}

bool Table::updateObstacleMap(const RelativeObstacleMap & relativeObstacleMap, Position & notrePosition, const Position & positionUncertainty)
{
	bool positionModified = false;

	DetectionPoint tabDetection[NB_CAPTEURS];
	fillDetectionPoints(tabDetection, notrePosition, relativeObstacleMap);
	interpreteDetectionPoints(tabDetection, notrePosition, positionUncertainty);
	positionModified = moveRobotToMatchFixedObstacles(tabDetection, notrePosition);
	moveObstaclesToMatchDetection(tabDetection);
	deleteUndetectedObstacles(tabDetection);
	addObstaclesToBeDeterminated(tabDetection);
	interpreteObstaclesInSight(tabDetection);

	return positionModified;
}

void Table::fillDetectionPoints(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const RelativeObstacleMap & relativeObstacleMap)
{
	if (NB_CAPTEURS != 8)
		return; // On sait jamais, ça évitera des SegFault ^^

	static Position point;
	static double theta;
	static int norme;

	/*
	##### Placement des points de détection #####
	*/
	// Capteur AVANT (n°0)
	theta = notrePosition.orientation;
	norme = relativeObstacleMap.avant + 42;
	point.x = norme*cos(theta);
	point.y = norme*sin(theta);
	point.x += notrePosition.x;
	point.y += notrePosition.y;
	tabDetection[0].x = point.x;
	tabDetection[0].y = point.y;
	tabDetection[0].isAnObstacle = !(relativeObstacleMap.avant == IR_INFINITY || relativeObstacleMap.avant == 0);
	tabDetection[0].isReliable = false;

	// Capteur ARRIERE (n°1)
	theta = notrePosition.orientation + PI;
	norme = relativeObstacleMap.arriere + 42;
	point.x = norme*cos(theta);
	point.y = norme*sin(theta);
	point.x += notrePosition.x;
	point.y += notrePosition.y;
	tabDetection[1].x = point.x;
	tabDetection[1].y = point.y;
	tabDetection[1].isAnObstacle = !(relativeObstacleMap.arriere == IR_INFINITY || relativeObstacleMap.arriere == 0);
	tabDetection[1].isReliable = false;

	// Capteur GAUCHE (n°2)
	theta = notrePosition.orientation + M_PI_2;
	norme = relativeObstacleMap.gauche + 35;
	point.x = norme*cos(theta);
	point.y = norme*sin(theta);
	point.x += notrePosition.x;
	point.y += notrePosition.y;
	tabDetection[2].x = point.x;
	tabDetection[2].y = point.y;
	tabDetection[2].isAnObstacle = !(relativeObstacleMap.gauche == TOF_INFINITY || relativeObstacleMap.gauche == 0);
	tabDetection[2].isReliable = true;

	// Capteur DROIT (n°3)
	theta = notrePosition.orientation - M_PI_2;
	norme = relativeObstacleMap.droit + 35;
	point.x = norme*cos(theta);
	point.y = norme*sin(theta);
	point.x += notrePosition.x;
	point.y += notrePosition.y;
	tabDetection[3].x = point.x;
	tabDetection[3].y = point.y;
	tabDetection[3].isAnObstacle = !(relativeObstacleMap.droit == TOF_INFINITY || relativeObstacleMap.droit == 0);
	tabDetection[3].isReliable = true;

	// Capteur AVANT GAUCHE (n°4)
	theta = 0.1745329251994; // angle de 10°
	norme = relativeObstacleMap.avantGauche;
	point.x = norme*cos(theta);
	point.y = norme*sin(theta);
	point.x += 37;
	point.y += 25;
	theta = notrePosition.orientation;
	point.x = point.x * cos(theta) - point.y * sin(theta);
	point.y = point.x * sin(theta) + point.y * cos(theta);
	point.x += notrePosition.x;
	point.y += notrePosition.y;
	tabDetection[4].x = point.x;
	tabDetection[4].y = point.y;
	tabDetection[4].isAnObstacle = !(relativeObstacleMap.avantGauche == TOF_INFINITY || relativeObstacleMap.avantGauche == 0);
	tabDetection[4].isReliable = true;

	// Capteur AVANT DROIT (n°5)
	theta = -0.1745329251994; // angle de -10°
	norme = relativeObstacleMap.avantDroit;
	point.x = norme*cos(theta);
	point.y = norme*sin(theta);
	point.x += 37;
	point.y -= 25;
	theta = notrePosition.orientation;
	point.x = point.x * cos(theta) - point.y * sin(theta);
	point.y = point.x * sin(theta) + point.y * cos(theta);
	point.x += notrePosition.x;
	point.y += notrePosition.y;
	tabDetection[5].x = point.x;
	tabDetection[5].y = point.y;
	tabDetection[5].isAnObstacle = !(relativeObstacleMap.avantDroit == TOF_INFINITY || relativeObstacleMap.avantDroit == 0);
	tabDetection[5].isReliable = true;

	// Capteur ARRIERE GAUCHE (n°6)
	theta = 2.96705972839036; // angle de PI - 10°
	norme = relativeObstacleMap.arriereGauche;
	point.x = norme*cos(theta);
	point.y = norme*sin(theta);
	point.x -= 37;
	point.y += 25;
	theta = notrePosition.orientation;
	point.x = point.x * cos(theta) - point.y * sin(theta);
	point.y = point.x * sin(theta) + point.y * cos(theta);
	point.x += notrePosition.x;
	point.y += notrePosition.y;
	tabDetection[6].x = point.x;
	tabDetection[6].y = point.y;
	tabDetection[6].isAnObstacle = !(relativeObstacleMap.arriereGauche == TOF_INFINITY || relativeObstacleMap.arriereGauche == 0);
	tabDetection[6].isReliable = true;

	// Capteur ARRIERE DROIT (n°7)
	theta = 3.31612557878922; // angle de PI + 10°
	norme = relativeObstacleMap.arriereDroit;
	point.x = norme*cos(theta);
	point.y = norme*sin(theta);
	point.x -= 37;
	point.y -= 25;
	theta = notrePosition.orientation;
	point.x = point.x * cos(theta) - point.y * sin(theta);
	point.y = point.x * sin(theta) + point.y * cos(theta);
	point.x += notrePosition.x;
	point.y += notrePosition.y;
	tabDetection[7].x = point.x;
	tabDetection[7].y = point.y;
	tabDetection[7].isAnObstacle = !(relativeObstacleMap.arriereDroit == TOF_INFINITY || relativeObstacleMap.arriereDroit == 0);
	tabDetection[7].isReliable = true;
}

void Table::interpreteDetectionPoints(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const Position & notreIncertitude)
{
	for (int i = 0; i < NB_CAPTEURS; i++)
	{
		tabDetection[i].associatedObstacleType = NONE;

		if (tabDetection[i].isAnObstacle)
		{
			// Movable Invisible
			for (size_t indiceObstacle = 0; indiceObstacle < obstacleMap.movableInvisible.size(); indiceObstacle++)
			{
				if (isThisPointThisObstacle(tabDetection[i], obstacleMap.movableInvisible.at(indiceObstacle), notreIncertitude))
				{
					tabDetection[i].associatedObstacleType = MOVABLE_INVISIBLE;
					tabDetection[i].associatedObstacle = indiceObstacle;
				}
			}

			// Fixed Invisible
			for (size_t indiceObstacle = 0; indiceObstacle < obstacleMap.fixedInvisible.size(); indiceObstacle++)
			{
				if (isThisPointThisObstacle(tabDetection[i], obstacleMap.fixedInvisible.at(indiceObstacle), notreIncertitude))
				{
					tabDetection[i].associatedObstacleType = FIXED_INVISIBLE;
					tabDetection[i].associatedObstacle = indiceObstacle;
				}
			}

			// To Be specified
			for (size_t indiceObstacle = 0; indiceObstacle < obstacleMap.toBeSpecified.size(); indiceObstacle++)
			{
				if (isThisPointThisObstacle(tabDetection[i], obstacleMap.toBeSpecified.at(indiceObstacle), notreIncertitude))
				{
					tabDetection[i].associatedObstacleType = TO_BE_SPECIFIED;
					tabDetection[i].associatedObstacle = indiceObstacle;
				}
			}

			// Movable Visible
			for (size_t indiceObstacle = 0; indiceObstacle < obstacleMap.movableVisible.size(); indiceObstacle++)
			{
				if (isThisPointThisObstacle(tabDetection[i], obstacleMap.movableVisible.at(indiceObstacle), notreIncertitude))
				{
					tabDetection[i].associatedObstacleType = MOVABLE_VISIBLE;
					tabDetection[i].associatedObstacle = indiceObstacle;
				}
			}

			// Fixed Visible
			for (size_t indiceObstacle = 0; indiceObstacle < obstacleMap.fixedVisible.size(); indiceObstacle++)
			{
				if (isThisPointThisObstacle(tabDetection[i], obstacleMap.fixedVisible.at(indiceObstacle), notreIncertitude))
				{
					tabDetection[i].associatedObstacleType = FIXED_VISIBLE;
					tabDetection[i].associatedObstacle = indiceObstacle;
				}
			}

			// Oponent Robot
			for (size_t indiceObstacle = 0; indiceObstacle < obstacleMap.oponentRobot.size(); indiceObstacle++)
			{
				if (isThisPointThisObstacle(tabDetection[i], obstacleMap.oponentRobot.at(indiceObstacle), notreIncertitude))
				{
					tabDetection[i].associatedObstacleType = OPONENT_ROBOT;
					tabDetection[i].associatedObstacle = indiceObstacle;
				}
			}
		}
	}
}

bool Table::isThisPointThisObstacle(const DetectionPoint & detectionPoint, const Obstacle & obstacle, const Position & uncertainty)
{
	Position centre;
	obstacle.getCenter(centre);

	if (obstacle.getShape() == CIRCLE)
	{
		float squaredUncertainty = square(uncertainty.x) + square(uncertainty.y);
		float squaredDistance = square(detectionPoint.x - centre.x) + square(detectionPoint.y - centre.y);

		return ABS(square(obstacle.getRadius()) - squaredDistance) <= squaredUncertainty;
	}
	else if (obstacle.getShape() == RECTANGLE)
	{
		if (centre.orientation == 0)
		{
			bool closeToXSide = (ABS(detectionPoint.x - obstacle.getXRadius()) <= uncertainty.x) && (ABS(centre.y - detectionPoint.y) <= obstacle.getYRadius() + uncertainty.y);
			bool closeToYSide = (ABS(detectionPoint.y - obstacle.getYRadius()) <= uncertainty.y) && (ABS(centre.x - detectionPoint.x) <= obstacle.getXRadius() + uncertainty.x);

			return closeToXSide || closeToYSide;
		}
		else
		{// L'obstacle a subi une rotation, on l'assimile donc désormai à un cercle
			float squaredUncertainty = square(uncertainty.x) + square(uncertainty.y);
			float squaredDistance = square(detectionPoint.x - centre.x) + square(detectionPoint.y - centre.y);

			return ABS(square(obstacle.getXRadius()) + square(obstacle.getYRadius()) - squaredDistance) <= squaredUncertainty;
		}
	}
	else if (obstacle.getShape() == EDGE_OF_TABLE)
	{
		bool closeToXEdge = ABS(detectionPoint.x - centre.x) >= obstacle.getXRadius() - uncertainty.x;
		bool closeToYEdge = ABS(detectionPoint.y - centre.y) >= obstacle.getYRadius() - uncertainty.y;

		return closeToXEdge || closeToYEdge;
	}

	return false;
}

bool Table::moveRobotToMatchFixedObstacles(DetectionPoint tabDetection[NB_CAPTEURS], Position & position)
{
	return false;
}

void Table::moveObstaclesToMatchDetection(DetectionPoint tabDetection[NB_CAPTEURS])
{
}

void Table::deleteUndetectedObstacles(DetectionPoint tabDetection[NB_CAPTEURS])
{
}

void Table::addObstaclesToBeDeterminated(DetectionPoint tabDetection[NB_CAPTEURS])
{
}

void Table::interpreteObstaclesInSight(DetectionPoint tabDetection[NB_CAPTEURS])
{
}

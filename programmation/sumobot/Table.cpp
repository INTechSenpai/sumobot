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

	// Support � coquillages n�1
	centre.x = 1500;
	centre.y = 0;
	cercle.setCenter(centre);
	cercle.setRadius(250);
	obstacleMap.fixedVisible.push_back(cercle);

	// Support � coquillages n�2
	centre.x = -1500;
	centre.y = 0;
	cercle.setCenter(centre);
	cercle.setRadius(250);
	obstacleMap.fixedVisible.push_back(cercle);



	/*
		##### El�ments de d�cors fixe #####
	*/
	
	// Tasseau de limite de la dune, c�t� x>0
	centre.x = 689;
	centre.y = 1900;
	rectangle.setCenter(centre);
	rectangle.setXRadius(11);
	rectangle.setYRadius(100);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Tasseau de limite de la dune, c�t� x<0
	centre.x = -689;
	centre.y = 1900;
	rectangle.setCenter(centre);
	rectangle.setXRadius(11);
	rectangle.setYRadius(100);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Tasseau de limite de NOTRE bac � sable
	if(side == GREEN)
		centre.x = 312;
	else
		centre.x = -312;
	centre.y = 761;
	rectangle.setCenter(centre);
	rectangle.setXRadius(288);
	rectangle.setYRadius(11);
	obstacleMap.fixedInvisible.push_back(rectangle);

	// Bac � sable adverse (inclu le tasseau de d�limitation)
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

	// Dune de sable (simplifi�e)
	centre.x = 0;
	centre.y = 1940;
	rectangle.setCenter(centre);
	rectangle.setXRadius(678);
	rectangle.setYRadius(60);
	obstacleMap.fixedInvisible.push_back(rectangle);


	/*
		##### El�ments de jeu #####
	*/

	// Tas de sable face � notre serviette
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
	
	// Gestion de la sym�trie
	int mirror;
	if (side == GREEN)
		mirror = 1;
	else
		mirror = -1;

	// De notre c�t�
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

	// C�t� adverse
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
	deleteUndetectedObstacles(tabDetection, notrePosition);
	addObstaclesToBeDeterminated(tabDetection, notrePosition);
	interpreteObstaclesInSight(tabDetection);
	deleteOutdatedObstacles();

	return positionModified;
}

void Table::fillDetectionPoints(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const RelativeObstacleMap & relativeObstacleMap)
{
	if (NB_CAPTEURS != 8)
		return; // On sait jamais, �a �vitera des SegFault ^^

	static Position point;
	static double theta;
	static int norme;

	/*
	##### Placement des points de d�tection #####
	*/
	// Capteur AVANT (n�0)
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

	// Capteur ARRIERE (n�1)
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

	// Capteur GAUCHE (n�2)
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

	// Capteur DROIT (n�3)
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

	// Capteur AVANT GAUCHE (n�4)
	theta = 0.1745329251994; // angle de 10�
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

	// Capteur AVANT DROIT (n�5)
	theta = -0.1745329251994; // angle de -10�
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

	// Capteur ARRIERE GAUCHE (n�6)
	theta = 2.96705972839036; // angle de PI - 10�
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

	// Capteur ARRIERE DROIT (n�7)
	theta = 3.31612557878922; // angle de PI + 10�
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
					obstacleMap.toBeSpecified.at(indiceObstacle).justSeen();
				}
			}

			// Movable Visible
			for (size_t indiceObstacle = 0; indiceObstacle < obstacleMap.movableVisible.size(); indiceObstacle++)
			{
				if (isThisPointThisObstacle(tabDetection[i], obstacleMap.movableVisible.at(indiceObstacle), notreIncertitude))
				{
					tabDetection[i].associatedObstacleType = MOVABLE_VISIBLE;
					tabDetection[i].associatedObstacle = indiceObstacle;
					obstacleMap.movableVisible.at(indiceObstacle).justSeen();
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
					obstacleMap.oponentRobot.at(indiceObstacle).justSeen();
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
		{// L'obstacle a subi une rotation, on l'assimile donc d�sormai � un cercle
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
	bool positionModified = false;
	bool bordDeTableVisible = false;

	/* On d�place le robot en fonction des obstacles fixes vus par les capteurs fiables */
	for (int i = 0; i < NB_CAPTEURS; i++)
	{
		if (tabDetection[i].associatedObstacleType == FIXED_VISIBLE && tabDetection[i].isReliable)
		{
			ObstacleShape obstacleShape = obstacleMap.fixedVisible.at(tabDetection[i].associatedObstacle).getShape();

			if (obstacleShape == CIRCLE || obstacleShape == EDGE_OF_TABLE)
			{
				bordDeTableVisible = true;
			}

			float xOffset = 0, yOffset = 0; // Le robot ainsi que tous les points de d�tection seront d�plac�s selon cet offset
			calculateOffsetToMatch(tabDetection[i], xOffset, yOffset);

			if (xOffset != 0 || yOffset != 0)
			{
				positionModified = true;
			}

			position.x += xOffset;
			position.y += yOffset;
			for (int j = 0; j < NB_CAPTEURS; j++)
			{
				tabDetection[j].x += xOffset;
				tabDetection[j].y += yOffset;
			}
		}
	}

	/* Si le robot est proche d'un bord de table (<150mm) il doit n�c�ssairement le voir */
	if (ABS(position.x) >= 1350 || ABS(position.y - 1000) >= 850)
	{
		if (!bordDeTableVisible)
		{ // Si on ne voit pas le bord de table, on �loigne le robot afin de d(robot, bordDeTable) = 200mm
			positionModified = true;

			if (position.x >= 1350)
			{
				position.x = 1300;
			}
			else if (position.x <= -1350)
			{
				position.x = -1300;
			}

			if (position.y >= 1850)
			{
				position.y = 1800;
			}
			else if (position.y <= 150)
			{
				position.y = 200;
			}
		}
	}

	return positionModified;
}

void Table::moveObstaclesToMatchDetection(DetectionPoint tabDetection[NB_CAPTEURS])
{
	for (int i = 0; i < NB_CAPTEURS; i++)
	{
		if (tabDetection[i].associatedObstacleType == MOVABLE_VISIBLE && tabDetection[i].isReliable)
		{
			float xOffset = 0, yOffset = 0;
			calculateOffsetToMatch(tabDetection[i], xOffset, yOffset);

			Position centre;
			obstacleMap.movableVisible.at(tabDetection[i].associatedObstacle).getCenter(centre);
			centre.x -= xOffset;
			centre.y -= yOffset;
			obstacleMap.movableVisible.at(tabDetection[i].associatedObstacle).setCenter(centre);
		}
	}
}

void Table::deleteUndetectedObstacles(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition)
{
	Position obstacleCenter;
	float squaredObstacleRadius;
	ObstacleShape shape;

	for (int i = 0; i < NB_CAPTEURS; i++)
	{// TODO faire confiance aux IR classiques � courte distance
		if (!tabDetection[i].isAnObstacle && tabDetection[i].isReliable)
		{
			for (size_t j = 0; j < obstacleMap.movableVisible.size(); j++)
			{
				obstacleMap.movableVisible.at(j).getCenter(obstacleCenter);
				shape = obstacleMap.movableVisible.at(j).getShape();
				if (shape == CIRCLE)
				{
					squaredObstacleRadius = square(obstacleMap.movableVisible.at(j).getRadius());
				}
				else
				{
					squaredObstacleRadius = square(obstacleMap.movableVisible.at(j).getXRadius()) + square(obstacleMap.movableVisible.at(j).getYRadius());
				}

				if (isObstacleInSight(notrePosition, obstacleCenter, squaredObstacleRadius, tabDetection[i].x, tabDetection[i].y))
				{
					obstacleMap.movableVisible.at(j).decreaseTTL(TTL_DECREASE_NOT_SEEN);
				}
			}

			for (size_t j = 0; j < obstacleMap.toBeSpecified.size(); j++)
			{
				obstacleMap.toBeSpecified.at(j).getCenter(obstacleCenter);
				shape = obstacleMap.toBeSpecified.at(j).getShape();
				if (shape == CIRCLE)
				{
					squaredObstacleRadius = square(obstacleMap.toBeSpecified.at(j).getRadius());
				}
				else
				{
					squaredObstacleRadius = square(obstacleMap.toBeSpecified.at(j).getXRadius()) + square(obstacleMap.toBeSpecified.at(j).getYRadius());
				}

				if (isObstacleInSight(notrePosition, obstacleCenter, squaredObstacleRadius, tabDetection[i].x, tabDetection[i].y))
				{
					obstacleMap.toBeSpecified.at(j).decreaseTTL(TTL_DECREASE_NOT_SEEN);
				}
			}

			for (size_t j = 0; j < obstacleMap.oponentRobot.size(); j++)
			{
				obstacleMap.oponentRobot.at(j).getCenter(obstacleCenter);
				shape = obstacleMap.oponentRobot.at(j).getShape();
				if (shape == CIRCLE)
				{
					squaredObstacleRadius = square(obstacleMap.oponentRobot.at(j).getRadius());
				}
				else
				{
					squaredObstacleRadius = square(obstacleMap.oponentRobot.at(j).getXRadius()) + square(obstacleMap.oponentRobot.at(j).getYRadius());
				}

				if (isObstacleInSight(notrePosition, obstacleCenter, squaredObstacleRadius, tabDetection[i].x, tabDetection[i].y))
				{
					obstacleMap.oponentRobot.at(j).decreaseTTL(TTL_DECREASE_NOT_SEEN);
				}
			}
		}
	}
}

void Table::addObstaclesToBeDeterminated(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition)
{
	for (int i = 0; i < NB_CAPTEURS; i++)
	{
		if (tabDetection[i].associatedObstacleType == NONE && tabDetection[i].isAnObstacle)
		{
			// On se place dans un rep�re centr� sur le robot
			float xDetection = tabDetection[i].x - notrePosition.x;
			float yDetection = tabDetection[i].y - notrePosition.y;
			float distanceToDetectionPoint = sqrt(square(xDetection) + square(yDetection));

			Position centreObstacle;
			centreObstacle.x = xDetection * (distanceToDetectionPoint + NEW_OBSTACLE_RADIUS) / distanceToDetectionPoint;
			centreObstacle.y = yDetection * (distanceToDetectionPoint + NEW_OBSTACLE_RADIUS) / distanceToDetectionPoint;
			centreObstacle.orientation = 0;
			
			// On revient dans le rep�re de la table
			centreObstacle.x += notrePosition.x;
			centreObstacle.y += notrePosition.y;

			Obstacle newObstacle(centreObstacle, CIRCLE);
			newObstacle.setRadius(NEW_OBSTACLE_RADIUS);
			newObstacle.justSeen();
			newObstacle.setTTL(TTL_TO_BE_SPECIFIED);

			obstacleMap.toBeSpecified.push_back(newObstacle);
			tabDetection[i].associatedObstacleType = TO_BE_SPECIFIED;
			tabDetection[i].associatedObstacle = obstacleMap.toBeSpecified.size() - 1;
		}
	}
}

void Table::interpreteObstaclesInSight(DetectionPoint tabDetection[NB_CAPTEURS])
{

}

void Table::calculateOffsetToMatch(const DetectionPoint & detectionPoint, float & xOffset, float & yOffset)
{
	Obstacle obstacle;
	switch (detectionPoint.associatedObstacleType)
	{
	case FIXED_VISIBLE:
		obstacle = obstacleMap.fixedVisible.at(detectionPoint.associatedObstacle);
		break;
	case FIXED_INVISIBLE:
		obstacle = obstacleMap.fixedInvisible.at(detectionPoint.associatedObstacle);
		break;
	case MOVABLE_VISIBLE:
		obstacle = obstacleMap.movableVisible.at(detectionPoint.associatedObstacle);
		break;
	case MOVABLE_INVISIBLE:
		obstacle = obstacleMap.movableInvisible.at(detectionPoint.associatedObstacle);
		break;
	case TO_BE_SPECIFIED:
		obstacle = obstacleMap.toBeSpecified.at(detectionPoint.associatedObstacle);
		break;
	case OPONENT_ROBOT:
		obstacle = obstacleMap.oponentRobot.at(detectionPoint.associatedObstacle);
		break;
	default:
		xOffset = 0;
		yOffset = 0;
		return;
		break;
	}

	Position centre;
	obstacle.getCenter(centre);

	float dx, dy; // Position du point de d�tection relativement au centre du rectangle
	dx = detectionPoint.x - centre.x;
	dy = detectionPoint.y - centre.y;

	if (obstacle.getShape() == CIRCLE)
	{
		float d; // Distance centre <--> point de d�tection
		d = sqrt(square(dx) + square(dy));

		xOffset = (obstacle.getRadius()) * dx / d - dx;
		yOffset = (obstacle.getRadius()) * dy / d - dy;
	}
	else
	{
		if (centre.orientation == 0)
		{
			float dxp, dxm, dyp, dym; // Distances point de d�tection <--> arr�tes du rectangle
			dxm = -(obstacle.getXRadius() + dx);
			dxp = obstacle.getXRadius() - dx;
			dym = -(obstacle.getYRadius() + dy);
			dyp = obstacle.getYRadius() - dy;

			if (ABS(dx) > obstacle.getXRadius() && ABS(dy) > obstacle.getYRadius())
			{
				if (dx > 0)
				{
					xOffset = dxp;
				}
				else
				{
					xOffset = dxm;
				}

				if (dy > 0)
				{
					yOffset = dyp;
				}
				else
				{
					yOffset = dym;
				}
			}
			else
			{
				if (ABS(dxm) < MIN(MIN(ABS(dxp), ABS(dym)), ABS(dyp)))
				{// dxm est le min en valeur absolue
					xOffset = dxm;
					yOffset = 0;
				}
				else if (ABS(dxp) < MIN(MIN(ABS(dxm), ABS(dym)), ABS(dyp)))
				{// dxp est le min en valeur absolue
					xOffset = dxp;
					yOffset = 0;
				}
				else if (ABS(dym) < MIN(MIN(ABS(dxp), ABS(dxm)), ABS(dyp)))
				{// dym est le min en valeur absolue
					xOffset = 0;
					yOffset = dym;
				}
				else
				{// dyp est le min en valeur absolue
					xOffset = 0;
					yOffset = dyp;
				}
			}
		}
		else
		{// Les rectanges inclin�s sont trait�s comme des cercles (le cercle circonscrit au rectangle)

			float radius; // Rayon du cercle circonscrit au rectangle
			float d; // Distance centre <--> point de d�tection
			
			radius = sqrt(square(obstacle.getXRadius()) + square(obstacle.getYRadius()));
			d = sqrt(square(dx) + square(dy));

			xOffset = radius * dx / d - dx;
			yOffset = radius * dy / d - dy;
		}
	}
}

bool Table::isObstacleInSight(const Position & robotCenter, const Position & obstacleCenter, float squaredObstacleRadius, float xHorizon, float yHorizon)
{
	float horizonLength = sqrt(square(xHorizon - robotCenter.x) + square(yHorizon - robotCenter.y));
	float squaredDistanceToObstacle = square(obstacleCenter.x - robotCenter.x) + square(obstacleCenter.y - robotCenter.y);
	
	float distanceToProjection = ((obstacleCenter.x - robotCenter.x) * (xHorizon - robotCenter.x) + (obstacleCenter.y - robotCenter.y) * (yHorizon - robotCenter.y)) / horizonLength;

	float squaredDistanceToLineOfSight = squaredDistanceToObstacle - square(distanceToProjection);

	return squaredDistanceToLineOfSight <= squaredObstacleRadius && (distanceToProjection <= horizonLength && distanceToProjection >= 0);
}

void Table::deleteOutdatedObstacles()
{
	for (size_t i = 0; i < obstacleMap.movableVisible.size(); i++)
	{
		if (obstacleMap.movableVisible.at(i).getTTL() <= millis() - obstacleMap.movableVisible.at(i).getLastTimeSeen())
			obstacleMap.movableVisible.erase(obstacleMap.movableVisible.begin() + i);
	}

	for (size_t i = 0; i < obstacleMap.oponentRobot.size(); i++)
	{
		if (obstacleMap.oponentRobot.at(i).getTTL() <= millis() - obstacleMap.oponentRobot.at(i).getLastTimeSeen())
			obstacleMap.oponentRobot.erase(obstacleMap.oponentRobot.begin() + i);
	}

	for (size_t i = 0; i < obstacleMap.toBeSpecified.size(); i++)
	{
		if (obstacleMap.toBeSpecified.at(i).getTTL() <= millis() - obstacleMap.toBeSpecified.at(i).getLastTimeSeen())
			obstacleMap.toBeSpecified.erase(obstacleMap.toBeSpecified.begin() + i);
	}
}

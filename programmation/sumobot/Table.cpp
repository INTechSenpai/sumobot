#include "Table.h"

Table::Table()
{
	enableUpdate = false;
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
	static uint32_t t1, t2;
	t1 = micros();

	bool positionModified = false;

	if (enableUpdate)
	{
		DetectionPoint tabDetection[NB_CAPTEURS];
		fillDetectionPoints(tabDetection, notrePosition, relativeObstacleMap);
		interpreteDetectionPoints(tabDetection, notrePosition, positionUncertainty);
		positionModified = moveRobotToMatchFixedObstacles(tabDetection, notrePosition);
		moveObstaclesToMatchDetection(tabDetection);
		deleteUndetectedObstacles(tabDetection, notrePosition);
		addObstaclesToBeDeterminated(tabDetection, notrePosition);
		interpreteObstaclesInSight(tabDetection, notrePosition);
		deleteOutdatedObstacles();
	}
	t2 = micros();

	/*
	Serial.printf("tbs:%d t=%d\n",
		obstacleMap.toBeSpecified.size(),
		t2 - t1);
	//*/

	return positionModified;
}

void Table::enableUpdateObstacleMap(bool enable)
{
	enableUpdate = enable;
	if (!enable)
	{
		obstacleMap.toBeSpecified.clear();
	}
}

size_t Table::getToBeSpecifiedLength()
{
	return obstacleMap.toBeSpecified.size();
}

void Table::fillDetectionPoints(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const RelativeObstacleMap & relativeObstacleMap)
{
	if (NB_CAPTEURS != 8)
		return; // On sait jamais, ça évitera des SegFault ^^

	static Position point;
	static double theta;
	static int norme;
	static float xRotated, yRotated;

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
	xRotated = point.x * cos(theta) - point.y * sin(theta);
	yRotated = point.x * sin(theta) + point.y * cos(theta);
	point.x = xRotated + notrePosition.x;
	point.y = yRotated + notrePosition.y;
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
	xRotated = point.x * cos(theta) - point.y * sin(theta);
	yRotated = point.x * sin(theta) + point.y * cos(theta);
	point.x = xRotated + notrePosition.x;
	point.y = yRotated + notrePosition.y;
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
	xRotated = point.x * cos(theta) - point.y * sin(theta);
	yRotated = point.x * sin(theta) + point.y * cos(theta);
	point.x = xRotated + notrePosition.x;
	point.y = yRotated + notrePosition.y;
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
	xRotated = point.x * cos(theta) - point.y * sin(theta);
	yRotated = point.x * sin(theta) + point.y * cos(theta);
	point.x = xRotated + notrePosition.x;
	point.y = yRotated + notrePosition.y;
	tabDetection[7].x = point.x;
	tabDetection[7].y = point.y;
	tabDetection[7].isAnObstacle = !(relativeObstacleMap.arriereDroit == TOF_INFINITY || relativeObstacleMap.arriereDroit == 0);
	tabDetection[7].isReliable = true;
}

void Table::interpreteDetectionPoints(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition, const Position & notreIncertitude)
{
	Position incertitudeAugmentee(notreIncertitude.x + NEW_OBSTACLE_RADIUS * 2, notreIncertitude.y + NEW_OBSTACLE_RADIUS * 2, notreIncertitude.orientation);

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
				if (isThisPointThisObstacle(tabDetection[i], obstacleMap.fixedVisible.at(indiceObstacle), incertitudeAugmentee))
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
	bool positionModified = false;
	bool bordDeTableVisible = false;

	/* On déplace le robot en fonction des obstacles fixes vus par les capteurs fiables */
	for (int i = 0; i < NB_CAPTEURS; i++)
	{
		if (tabDetection[i].associatedObstacleType == FIXED_VISIBLE && tabDetection[i].isReliable)
		{
			ObstacleShape obstacleShape = obstacleMap.fixedVisible.at(tabDetection[i].associatedObstacle).getShape();

			if (obstacleShape == CIRCLE || obstacleShape == EDGE_OF_TABLE)
			{
				bordDeTableVisible = true;
			}

			float xOffset = 0, yOffset = 0; // Le robot ainsi que tous les points de détection seront déplacés selon cet offset
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

	/* Si le robot est proche d'un bord de table (<150mm) il doit nécéssairement le voir */
	if (ABS(position.x) >= 1350 || ABS(position.y - 1000) >= 850)
	{
		if (!bordDeTableVisible)
		{ // Si on ne voit pas le bord de table, on éloigne le robot afin que d(robot, bordDeTable) = 200mm
			positionModified = true;

			float xOffset = 0, yOffset = 0;

			if (position.x >= 1350)
			{
				xOffset = 1300 - position.x;
			}
			else if (position.x <= -1350)
			{
				xOffset = -1300 - position.x;
			}

			if (position.y >= 1850)
			{
				yOffset = 1800 - position.y;
			}
			else if (position.y <= 150)
			{
				yOffset = 200 - position.y;
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
		else if (tabDetection[i].associatedObstacleType == TO_BE_SPECIFIED && tabDetection[i].isReliable)
		{
			float xOffset = 0, yOffset = 0;
			calculateOffsetToMatch(tabDetection[i], xOffset, yOffset);

			Position centre;
			obstacleMap.toBeSpecified.at(tabDetection[i].associatedObstacle).getCenter(centre);
			centre.x -= xOffset;
			centre.y -= yOffset;
			obstacleMap.toBeSpecified.at(tabDetection[i].associatedObstacle).setCenter(centre);
		}
		
	}
}

void Table::deleteUndetectedObstacles(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition)
{
	Position obstacleCenter;
	float squaredObstacleRadius;
	ObstacleShape shape;

	for (int i = 0; i < NB_CAPTEURS; i++)
	{// TODO faire confiance aux IR classiques à courte distance
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
			// On se place dans un repère centré sur le robot
			float xDetection = tabDetection[i].x - notrePosition.x;
			float yDetection = tabDetection[i].y - notrePosition.y;
			float distanceToDetectionPoint = sqrt(square(xDetection) + square(yDetection));

			Position centreObstacle;
			centreObstacle.x = xDetection * (distanceToDetectionPoint + NEW_OBSTACLE_RADIUS) / distanceToDetectionPoint;
			centreObstacle.y = yDetection * (distanceToDetectionPoint + NEW_OBSTACLE_RADIUS) / distanceToDetectionPoint;
			centreObstacle.orientation = 0;
			
			// On revient dans le repère de la table
			centreObstacle.x += notrePosition.x;
			centreObstacle.y += notrePosition.y;

			//Serial.printf("#NEW#(%d) xd:%g  yd:%g  xO:%g  yO:%g (x:%g  y:%g) t:%d\n", i, xDetection, yDetection, centreObstacle.x, centreObstacle.y, notrePosition.x, notrePosition.y, obstacleMap.toBeSpecified.size());

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

void Table::interpreteObstaclesInSight(DetectionPoint tabDetection[NB_CAPTEURS], const Position & notrePosition)
{
	// Si les deux capteurs ToF avant détectent un 'ToBeSpecified'
	if (tabDetection[4].associatedObstacleType == TO_BE_SPECIFIED && tabDetection[5].associatedObstacleType == TO_BE_SPECIFIED)
	{
		if (tabDetection[0].associatedObstacleType == TO_BE_SPECIFIED)
		{
			specifyAsOponentRobot(tabDetection[4].associatedObstacle, tabDetection[0].associatedObstacle, tabDetection[5].associatedObstacle, notrePosition);
			return;
		}
		else
		{
			specifyAsOponentRobot(tabDetection[4].associatedObstacle, tabDetection[4].associatedObstacle, tabDetection[5].associatedObstacle, notrePosition);
			return;
		}
	}

	// Si les deux capteurs ToF arrière détectent un 'ToBeSpecified'
	if (tabDetection[6].associatedObstacleType == TO_BE_SPECIFIED && tabDetection[7].associatedObstacleType == TO_BE_SPECIFIED)
	{
		if (tabDetection[1].associatedObstacleType == TO_BE_SPECIFIED)
		{
			specifyAsOponentRobot(tabDetection[6].associatedObstacle, tabDetection[1].associatedObstacle, tabDetection[7].associatedObstacle, notrePosition);
			return;
		}
		else
		{
			specifyAsOponentRobot(tabDetection[6].associatedObstacle, tabDetection[6].associatedObstacle, tabDetection[7].associatedObstacle, notrePosition);
			return;
		}
	}

	if ((!tabDetection[4].isAnObstacle && !tabDetection[5].isAnObstacle) && tabDetection[0].associatedObstacleType == TO_BE_SPECIFIED)
	{
		if (square(tabDetection[0].x - notrePosition.x) + square(tabDetection[0].y - notrePosition.y) < square(TOF_INFINITY))
		{
			specifyAsMovable(tabDetection[0].associatedObstacle);
			return;
		}
	}

	if ((!tabDetection[6].isAnObstacle && !tabDetection[7].isAnObstacle) && tabDetection[1].associatedObstacleType == TO_BE_SPECIFIED)
	{
		if (square(tabDetection[1].x - notrePosition.x) + square(tabDetection[1].y - notrePosition.y) < square(TOF_INFINITY))
		{
			specifyAsMovable(tabDetection[1].associatedObstacle);
			return;
		}
	}
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

	float dx, dy; // Position du point de détection relativement au centre du rectangle
	dx = detectionPoint.x - centre.x;
	dy = detectionPoint.y - centre.y;

	if (obstacle.getShape() == CIRCLE)
	{
		float d; // Distance centre <--> point de détection
		d = sqrt(square(dx) + square(dy));

		xOffset = (obstacle.getRadius()) * dx / d - dx;
		yOffset = (obstacle.getRadius()) * dy / d - dy;
	}
	else
	{
		if (centre.orientation == 0)
		{
			float dxp, dxm, dyp, dym; // Distances point de détection <--> arrêtes du rectangle
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
		{// Les rectanges inclinés sont traités comme des cercles (le cercle circonscrit au rectangle)

			float radius; // Rayon du cercle circonscrit au rectangle
			float d; // Distance centre <--> point de détection
			
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

void Table::specifyAsOponentRobot(size_t leftObstacle, size_t frontObstacle, size_t rightObstacle, const Position & notrePosition)
{
	Position centreObstacle;
	obstacleMap.toBeSpecified.at(frontObstacle).getCenter(centreObstacle);
	Position centreRobotAdverse;
	float distanceRobotObstacle = sqrt(square(centreObstacle.x - notrePosition.x) + square(centreObstacle.y - notrePosition.y));

	//On se place dans un repère centré sur notre robot
	centreRobotAdverse.x = centreObstacle.x - notrePosition.x;
	centreRobotAdverse.y = centreObstacle.y - notrePosition.y;
	centreRobotAdverse.orientation = 0;

	float homotetie = (distanceRobotObstacle + OPONENT_RADIUS - obstacleMap.toBeSpecified.at(frontObstacle).getRadius()) / distanceRobotObstacle;

	centreRobotAdverse.x *= homotetie;
	centreRobotAdverse.y *= homotetie;

	//On revient dans le repère de la table
	centreRobotAdverse.x += notrePosition.x;
	centreRobotAdverse.y += notrePosition.y;

	Obstacle robotAdverse(centreRobotAdverse, CIRCLE);
	robotAdverse.setRadius(OPONENT_RADIUS);
	robotAdverse.setTTL(TTL_OPONENT_ROBOT);
	robotAdverse.justSeen();

	// On trie les indices, pour les supprimer dans un ordre qui ne pose pas problème... --"
	size_t tabIndice[3];
	tabIndice[0] = leftObstacle;
	tabIndice[1] = frontObstacle;
	tabIndice[2] = rightObstacle;

	for (int i = 0; i <= 1; i++)
	{
		int indiceMax = i;
		for (int j = i + 1; j <= 2; j++)
		{
			if (tabIndice[j] > tabIndice[indiceMax])
			{
				indiceMax = j;
			}
		}
		size_t mem = tabIndice[i];
		tabIndice[i] = tabIndice[indiceMax];
		tabIndice[indiceMax] = mem;
	}

	obstacleMap.toBeSpecified.erase(obstacleMap.toBeSpecified.begin() + tabIndice[0]);
	for (int i = 1; i < 3; i++)
	{
		if (tabIndice[i - 1] != tabIndice[i])
			obstacleMap.toBeSpecified.erase(obstacleMap.toBeSpecified.begin() + tabIndice[i]);
	}

	obstacleMap.oponentRobot.push_back(robotAdverse);
}

void Table::specifyAsMovable(size_t indiceObstacle)
{
	obstacleMap.movableVisible.push_back(obstacleMap.toBeSpecified.at(indiceObstacle));
	obstacleMap.toBeSpecified.erase(obstacleMap.toBeSpecified.begin() + indiceObstacle);
}

bool Table::isObstacleOnTrajectory(const Obstacle & obstacle, const UnitMove & unitMove, const Position & notrePosition, float moveProgress)
{
	Position centreObstacle;
	obstacle.getCenter(centreObstacle);
	float rObstacle = obstacle.getRadius() + ROBOT_EXT_RADIUS;
	float moveLength = unitMove.getLengthMm() * (1 - moveProgress);
	if (moveLength < 100 && moveLength >= 0)
	{
		moveLength = 100;
	}
	else if (moveLength > 300)
	{
		moveLength = 300;
	}
	else if (moveLength > -100 && moveLength < 0)
	{
		moveLength = -100;
	}
	else if (moveLength < -300)
	{
		moveLength = -300;
	}



	if (unitMove.getBendRadiusTicks() == 0)
	{
		return false;
	}
	else
	{
		// On calcule les coordonnées du point C, centre du cercle sur lequel on se déplace.
		Position centreCourbure;
		centreCourbure.x = -sin(notrePosition.orientation) * unitMove.getBendRadiusMm();
		centreCourbure.y = cos(notrePosition.orientation) * unitMove.getBendRadiusMm();

		// Vérification de l'existence de points d'intersection
		if (square(centreCourbure.x - centreObstacle.x) + square(centreCourbure.y - centreObstacle.y) > square(unitMove.getBendRadiusMm() + rObstacle))
		{// Pas de point d'intersection
			return false;
		}

		// Calcul des coordonnées des points d'intersections des deux cercles (trajectoire et obstacle)
		float a = 2 * (centreCourbure.x - centreObstacle.x);
		float sqA = square(a);
		float b = 2 * (centreCourbure.y - centreObstacle.y);
		if (b == 0)
		{// Permet d'éviter les cas spéciaux sans perte de précision
			b = 1;// (uinté : mm)
		}
		float sqB = square(b);
		float c = square(centreCourbure.x - centreObstacle.x) + square(centreCourbure.y - centreObstacle.y) - square(unitMove.getBendRadiusMm()) + square(rObstacle);
		float sqC = square(c);
		float delta = 4 * sqA * sqC - (4 * (sqA + sqB) * (sqC - sqB * square(rObstacle)));

		if (delta < 0 || (a == 0 && b == 0))
		{
			return false;
		}

		// Positions des points d'intersection
		Position I1, I2;

		I1.x = centreObstacle.x + (2*a*c - sqrt(delta)) / (2 * (sqA + sqB));
		I2.x = centreObstacle.x + (2*a*c + sqrt(delta)) / (2 * (sqA + sqB));

		I1.y = centreObstacle.y + (c - a * (I1.x - centreObstacle.x)) / b;
		I2.y = centreObstacle.y + (c - a * (I2.x - centreObstacle.x)) / b;


		// Le point R : notre position
		// Le point F : la fin de notre mouvement unitaire
		// Les vecteurs CR et CF représentent respectivement les délimitations du début et de la fin de l'arc trajectoire.
		Position CR;
		CR.x = notrePosition.x - centreCourbure.x;
		CR.y = notrePosition.y - centreCourbure.y;

		// Soit alpha l'angle orienté entre CR et CF
		float alpha = moveLength / unitMove.getBendRadiusMm();
		float cosAlpha = cos(alpha);
		float sinAlpha = sin(alpha);

		Position CF;
		CF.x = CR.x * cosAlpha - CR.y * sinAlpha;
		CF.y = CR.x * sinAlpha + CR.y * cosAlpha;

		// On fait subir une rotation de +PI/2 à CF et CR afin d'avoir leurs horthogonaux
		float mem;
		mem = CR.x;
		CR.x = -CR.y;
		CR.y = mem;
		mem = CF.x;
		CF.x = -CF.y;
		CF.y = mem;

		// On transforme les points I1 et I2 en des vecteurs CI1 et CI2
		I1.x -= centreCourbure.x;
		I1.y -= centreCourbure.y;
		I2.x -= centreCourbure.x;
		I2.y -= centreCourbure.y;

		bool obstacleOnTrajectory;
		if (moveLength * unitMove.getBendRadiusTicks() < 0)
		{
			obstacleOnTrajectory =
				(CF.x * I1.x + CF.y * I1.y > 0 && CR.x * I1.x + CR.y * I1.y < 0) ||
				(CF.x * I2.x + CF.y * I2.y > 0 && CR.x * I2.x + CR.y * I2.y < 0);
		}
		else
		{
			obstacleOnTrajectory =
				(CF.x * I1.x + CF.y * I1.y < 0 && CR.x * I1.x + CR.y * I1.y > 0) ||
				(CF.x * I2.x + CF.y * I2.y < 0 && CR.x * I2.x + CR.y * I2.y > 0);
		}

		return obstacleOnTrajectory;
	}
}

bool Table::isTrajectoryAllowed(const Trajectory & trajectory, uint32_t currentMove, const Position & notrePosition, float moveProgress)
{
	if (currentMove < trajectory.size())
	{
		for (int i = 0; i < obstacleMap.oponentRobot.size(); i++)
		{
			if (isObstacleOnTrajectory(obstacleMap.oponentRobot.at(i), trajectory.at(currentMove), notrePosition, moveProgress))
			{
				return false;
			}
		}

		for (int i = 0; i < obstacleMap.toBeSpecified.size(); i++)
		{
			if (isObstacleOnTrajectory(obstacleMap.toBeSpecified.at(i), trajectory.at(currentMove), notrePosition, moveProgress))
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		return true;
	}
}

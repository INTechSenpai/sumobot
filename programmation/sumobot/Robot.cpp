#include "Robot.h"

Robot::Robot() : 
	table(Table::Instance()),
	sensorMgr(SensorMgr::Instance()),
	motionControlSystem(MotionControlSystem::Instance())
{
}

Side Robot::checkSide()
{
	Side side;
	RelativeObstacleMap relativeObstacleMap;
	uint32_t leftCounter = 0, rightCounter = 0;

	// Attente de la disponibilité des capteurs (et de la présence d'un bord de table)
	do
	{
		sensorMgr.getRelativeObstacleMapNoReset(relativeObstacleMap);
		delay(30);
	} while (
		(relativeObstacleMap.gauche == 255 || relativeObstacleMap.gauche == 0) && 
		(relativeObstacleMap.droit == 255 || relativeObstacleMap.droit == 0));


	for (int i = 0; i < 50; i++)
	{
		sensorMgr.getRelativeObstacleMap(relativeObstacleMap);

		if (relativeObstacleMap.gauche == 0)
			relativeObstacleMap.gauche = 256;
		if (relativeObstacleMap.droit == 0)
			relativeObstacleMap.droit = 256;

		if (relativeObstacleMap.gauche < relativeObstacleMap.droit)
		{
			leftCounter++;
		}
		else
		{
			rightCounter++;
		}

		delay(30);
	}

	uint8_t pinDEL;

	if (leftCounter < rightCounter)
	{
		side = GREEN;
		pinDEL = PIN_DEL_GREEN;
	}
	else
	{
		side = PURPLE;
		pinDEL = PIN_DEL_RED;
	}

	pinMode(pinDEL, OUTPUT);
	for (int i = 0; i < 3; i++)
	{
		digitalWrite(pinDEL, HIGH);
		delay(500);
		digitalWrite(pinDEL, LOW);
		delay(500);
	}

	return side;
}

void Robot::init(Side side)
{
	table.initObstacleMap(side);

	Position positionInitiale, incertitudeInitiale;
	if(side == GREEN)
		positionInitiale.x = 1350;
	else
		positionInitiale.x = -1350;
	positionInitiale.y = 1150;
	positionInitiale.orientation = PI / 2;

	incertitudeInitiale.x = 5;
	incertitudeInitiale.y = 5;
	incertitudeInitiale.orientation = 0;

	motionControlSystem.setPosition(positionInitiale);
	motionControlSystem.setPositionUncertainty(incertitudeInitiale);
}

void Robot::waitForBegining()
{
	pinMode(PIN_DEL_ONBOARD, OUTPUT);

	while (analogRead(PIN_JUMPER) < 80)
	{// Tant que le jumper n'est pas inséré
		blinkDelOnBoard(250, 250);
	}
	while (analogRead(PIN_JUMPER) > 20)
	{
		blinkDelOnBoard(1000, 1000);
	}
	blinkDelOnBoard(0, 1000);
}

void Robot::winMatch(uint32_t duration)
{
	uint32_t beginTime = millis();
	while (millis() - beginTime < duration)
	{
		Serial.println("Je gagne le match !!");
		delay(100);
	}
	Serial.println("Match fini !");
}

void Robot::deployUmbrella()
{
	uint8_t pinCommandeParasol = 28;

	pinMode(pinCommandeParasol, OUTPUT);
	digitalWrite(pinCommandeParasol, HIGH);
	delay(1000);
	digitalWrite(pinCommandeParasol, LOW);
}

bool Robot::goToPoint(const Position & destination)
{
	Position notrePosition;
	Trajectory trajectoire;

	uint32_t beginTime = 0;
	
	while (true)
	{
		beginTime = millis();
		motionControlSystem.getPosition(notrePosition);
		if (areWeArrived(notrePosition, destination))
		{
			break;
		}
		else
		{
			obstacleMap = table.getObstacleMap();
			trajectoire = pathfinding.computePath(obstacleMap, notrePosition, destination, 0);
			if (trajectoire.size() == 0)
				return false;
			else
				motionControlSystem.setTrajectory(trajectoire);
		}

		while (millis() - beginTime < 100);
	}
	return true;
}

bool Robot::areWeArrived(const Position & notrePosition, const Position & destination)
{
	return 
		(ABS(notrePosition.x - destination.x) < XY_TOLERANCE) && 
		(ABS(notrePosition.y - destination.y) < XY_TOLERANCE) && 
		(ABS(modulo(notrePosition.orientation*1000, 6283) - modulo(destination.orientation*1000, 6283)) < ANGLE_TOLERANCE*1000);
}

void Robot::driveAlongEdgeOfTable(Side side)
{
}

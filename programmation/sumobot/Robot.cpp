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

	// Attente de la disponibilit� des capteurs (et de la pr�sence d'un bord de table)
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
	{// Tant que le jumper n'est pas ins�r�
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
			trajectoire = pathfinding.computePath(obstacleMap, notrePosition, destination);
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
	// D�termine la fr�quence d'asservissement
	const uint32_t delaiAsservissement = 100; // P�riode d'asservissement, en ms
	uint32_t beginTime; // Permet le r�glage de la p�riode dans la boucle d'asservissement

	RelativeObstacleMap allValues; // Pour r�cup�rer les valeurs des capteurs
	volatile int32_t sensorValue;	// Distance au bord de table mesur�e, en mm
	volatile int32_t aimValue;		// Distance voulue, en mm
	volatile int32_t output;		// Indique � quel point il faudra tourner.
	int32_t frontDistance;			// Distance au bord faisant face au robot, indiquant quand il faudra s'arr�ter

	PID sensorPID(&sensorValue, &output, &aimValue); // PID pour asservissement sur la distance au bord de table;

	Trajectory trajectoireAsservie;
	UnitMove courbeAsservie;
	courbeAsservie.setLengthMm(100); // Longueur suffisante pour que le mouvement ne se termine pas entre deux asservissements de trajectoire
	courbeAsservie.setBendRadiusMm(INFINITE_RADIUS); // Valeur initiale, qui sera modifi�e par le PID
	courbeAsservie.setSpeedMm_S(350); // Vitesse du mouvement, la vitesse la plus fiable et test�e a �t� choisie
	courbeAsservie.stopAfterMove = false; // Inutile de s'arr�ter entre les mouvements
	trajectoireAsservie.push_back(courbeAsservie);

	/* La distance au bord doit �tre de 15cm, le capteur doit donc lire 115mm */
	aimValue = 115;

	/* R�glage des constantes d'asservissement */
	sensorPID.setTunings(1, 0, 0);

	// Boucle d'asservissement
	do
	{
		beginTime = millis();

		sensorMgr.getRelativeObstacleMapNoReset(allValues);
		if (side == GREEN)
		{
			sensorValue = allValues.droit;
		}
		else
		{
			sensorValue = allValues.gauche;
		}
		sensorPID.compute(); // Met � jour la valeur de 'output'
		
		if (side == PURPLE)
		{
			output = -output;
		}

		if (output == 0)
		{
			trajectoireAsservie.at(0).setBendRadiusMm(INFINITE_RADIUS);
		}
		else
		{
			trajectoireAsservie.at(0).setBendRadiusMm(2000 / output);
		}
		motionControlSystem.setTrajectory(trajectoireAsservie);

		frontDistance = calculateFrontDistance(allValues.avantGauche, allValues.avant, allValues.avantDroit);

		while (millis() - beginTime < delaiAsservissement);
	} while (frontDistance > 237);

	// Mouvement final (permettant l'arr�t)
	trajectoireAsservie.at(0).setBendRadiusMm(INFINITE_RADIUS);
	trajectoireAsservie.at(0).setLengthMm(40);
	trajectoireAsservie.at(0).stopAfterMove = true;
	motionControlSystem.setTrajectory(trajectoireAsservie);
	while (motionControlSystem.isMoving());
	
	// Une fois le robot arr�t�, on r�gle pr�cis�ment la position � l'aide des capteurs
	sensorMgr.getRelativeObstacleMapNoReset(allValues);
	frontDistance = calculateFrontDistance(allValues.avantGauche, allValues.avant, allValues.avantDroit);
	Position newPosition;
	if (side == GREEN)
	{
		newPosition.x = 1500 - allValues.droit - 35;
	}
	else
	{
		newPosition.x = -1500 + allValues.gauche + 35;
	}
	newPosition.y = 2000 - frontDistance;
	newPosition.orientation = M_PI_2;
	motionControlSystem.setPosition(newPosition);
	
	// On r�gle �galement l'incertitude sur la position
	Position newUncertainty;
	newUncertainty.x = 0;
	newUncertainty.y = 0;
	newUncertainty.orientation = 0;
	motionControlSystem.setPositionUncertainty(newUncertainty);
}

int32_t Robot::calculateFrontDistance(uint32_t gauche, uint32_t centre, uint32_t droite)
{
	// Offset pour avoir la distance par rapport au centre du robot
	gauche += 37;
	droite += 37;
	centre += 42;

	if (gauche == TOF_INFINITY && centre == IR_INFINITY && droite == TOF_INFINITY)
	{// Pas d'obstacle avant l'horizon
		return IR_INFINITY;
	}
	else if (gauche == TOF_INFINITY && droite == TOF_INFINITY)
	{// Obstacle lointain
		return centre;
	}
	else if (gauche == TOF_INFINITY)
	{// Cas � la con n�1
		if (centre == IR_INFINITY)
		{
			return droite;
		}
		else
		{
			return (centre + droite) / 2;
		}
	}
	else if (droite == TOF_INFINITY)
	{// Cas � la con n�2
		if (centre == IR_INFINITY)
		{
			return gauche;
		}
		else
		{
			return (centre + gauche) / 2;
		}
	}
	else
	{// Obstacle proche vu par tous les capteurs
		return (gauche + droite) / 2; // Le capteur central manque de pr�cision � cette distance
	}
}

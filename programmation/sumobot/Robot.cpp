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

	pinMode(PIN_DEL_GREEN, OUTPUT);
	pinMode(PIN_DEL_RED, OUTPUT);
	bool allume = true;

	// Attente de la disponibilit� des capteurs (et de la pr�sence d'un bord de table)
	do
	{
		digitalWrite(PIN_DEL_GREEN, allume);
		digitalWrite(PIN_DEL_RED, !allume);
		allume = !allume;

		sensorMgr.getRelativeObstacleMapNoReset(relativeObstacleMap);
		delay(100);
	} while (
		(relativeObstacleMap.gauche == 255 || relativeObstacleMap.gauche == 0) && 
		(relativeObstacleMap.droit == 255 || relativeObstacleMap.droit == 0));

	digitalWrite(PIN_DEL_GREEN, LOW);
	digitalWrite(PIN_DEL_RED, LOW);

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

	for (int i = 0; i < 3; i++)
	{
		digitalWrite(pinDEL, HIGH);
		delay(300);
		digitalWrite(pinDEL, LOW);
		delay(300);
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

	incertitudeInitiale.x = 30;
	incertitudeInitiale.y = 60;
	incertitudeInitiale.orientation = 0;

	motionControlSystem.setPosition(positionInitiale);
	motionControlSystem.setPositionUncertainty(incertitudeInitiale);

	table.enableUpdateObstacleMap(true);
}

void Robot::waitForBegining()
{
	int value = 0;
	pinMode(PIN_DEL_ONBOARD, OUTPUT);

	while (value < 80)
	{// Tant que le jumper n'est pas ins�r�
		delay(50);
		blinkDelOnBoard(250, 250);
		value = analogRead(PIN_JUMPER);
	}
	while (value > 20)
	{
		delay(50);
		blinkDelOnBoard(1000, 1000);
		value = analogRead(PIN_JUMPER);
	}
	blinkDelOnBoard(0, 1000);
}

void Robot::winMatch(uint32_t duration, Side side)
{
	uint32_t beginTime = millis();

	driveAlongEdgeOfTable(side);
	if (millis() - beginTime > duration - 1000)
	{
		return;
	}

	scriptCloseDoors(side);
	if (millis() - beginTime > duration - 5000)
	{
		return;
	}

	scriptGoToTowelFromDoors(side);
	if (millis() - beginTime > duration - 3000)
	{
		return;
	}

	scriptPushSand(side);

	scriptYolo(side, duration - millis() + beginTime);

	while (millis() - beginTime < duration)
	{
		
	}
	Serial.println("Match fini !");
}

void Robot::deployUmbrella()
{
	uint8_t pinCommandeParasol = 28;

	pinMode(pinCommandeParasol, OUTPUT);
	for (int i = 0; i < 3; i++)
	{
		digitalWrite(pinCommandeParasol, HIGH);
		delay(1000);
		digitalWrite(pinCommandeParasol, LOW);
		delay(1000);
	}
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

void Robot::goStraightToPoint(const Position & destination, int32_t speed)
{
	Position ici;
	motionControlSystem.getPosition(ici);
	Trajectory trajectoire = dummyPathFinding.computePath(ici, destination, speed, false);
	motionControlSystem.setTrajectory(trajectoire);
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
	const uint32_t delaiAsservissement = 25; // P�riode d'asservissement, en ms
	uint32_t beginTime; // Permet le r�glage de la p�riode dans la boucle d'asservissement

	RelativeObstacleMap allValues; // Pour r�cup�rer les valeurs des capteurs
	volatile int32_t sensorValue;	// Distance au bord de table mesur�e, en mm
	volatile int32_t aimValue;		// Distance voulue, en mm
	volatile int32_t output = 0;	// Indique � quel point il faudra tourner.
	int32_t frontDistance;			// Distance au bord faisant face au robot, indiquant quand il faudra s'arr�ter
	Position notrePosition;

	PID sensorPID(&sensorValue, &output, &aimValue); // PID pour asservissement sur la distance au bord de table;

	Trajectory trajectoireAsservie;
	UnitMove courbeAsservie;
	courbeAsservie.setLengthMm(200); // Longueur suffisante pour que le mouvement ne se termine pas entre deux asservissements de trajectoire
	courbeAsservie.setBendRadiusMm(INFINITE_RADIUS); // Valeur initiale, qui sera modifi�e par le PID
	courbeAsservie.setSpeedMm_S(250); // Vitesse du mouvement, la vitesse la plus fiable et test�e a �t� choisie
	courbeAsservie.stopAfterMove = false; // Inutile de s'arr�ter entre les mouvements
	trajectoireAsservie.push_back(courbeAsservie);

	/* La distance au bord doit �tre de 15cm, le capteur doit donc lire 115mm */
	aimValue = 115;

	/* R�glage des constantes d'asservissement */
	sensorPID.setTunings(0.5, 0, 5);

	sensorPID.setOutputLimits(-100, 100); // Permet d'interdire les rayons de courbure trop faibles (qui cassent tout ^^)

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
			trajectoireAsservie.at(0).setBendRadiusMm(20000 / output);
		}
		motionControlSystem.setTrajectory(trajectoireAsservie);
		motionControlSystem.getPosition(notrePosition);

		frontDistance = calculateFrontDistance(allValues.avantGauche, allValues.avant, allValues.avantDroit);

		while (millis() - beginTime < delaiAsservissement);
	} while (frontDistance > 300 || notrePosition.y < 1560);
	
	// Mouvement final (permettant l'arr�t)
	trajectoireAsservie.at(0).setBendRadiusMm(INFINITE_RADIUS);
	trajectoireAsservie.at(0).setLengthMm(30);
	trajectoireAsservie.at(0).stopAfterMove = true;
	motionControlSystem.setTrajectory(trajectoireAsservie);
	while (motionControlSystem.isMoving());
	
	delay(200);

	// Une fois le robot arr�t�, on r�gle pr�cis�ment la position � l'aide des capteurs
	sensorMgr.getRelativeObstacleMapNoReset(allValues);
	frontDistance = calculateFrontDistance(allValues.avantGauche, allValues.avant, allValues.avantDroit);
	float frontAngle = calculateFrontAngle(allValues.avantGauche, allValues.avantDroit);
	float cosFrontAngle = cos(frontAngle);
	Position newPosition;
	if (side == GREEN)
	{
		newPosition.x = 1500 - (((float)allValues.droit + 35) * cosFrontAngle);
	}
	else
	{
		newPosition.x = -1500 + (((float)allValues.gauche + 35) * cosFrontAngle);
	}
	newPosition.y = 2000 - (frontDistance * cosFrontAngle);
	newPosition.orientation = M_PI_2 + frontAngle;

	motionControlSystem.setPosition(newPosition);
	
	// On r�gle �galement l'incertitude sur la position
	Position newUncertainty;
	newUncertainty.x = 50;
	newUncertainty.y = 50;
	newUncertainty.orientation = 0;
	motionControlSystem.setPositionUncertainty(newUncertainty);
}

int32_t Robot::calculateFrontDistance(uint32_t gauche, uint32_t centre, uint32_t droite)
{
	// Gestion (imparfaite) du cas d'erreur
	if (gauche == 0)
		gauche = TOF_INFINITY;
	if (droite == 0)
		droite = TOF_INFINITY;
	if (centre == 0)
		centre = IR_INFINITY;

	// Offset pour avoir la distance par rapport au centre du robot
	const int ir_offset = 42;
	const int tof_offset = 37;


	if (gauche == TOF_INFINITY && centre == IR_INFINITY && droite == TOF_INFINITY)
	{// Pas d'obstacle avant l'horizon
		return IR_INFINITY + ir_offset;
	}
	else if (gauche == TOF_INFINITY && droite == TOF_INFINITY)
	{// Obstacle lointain
		if (centre + ir_offset > TOF_INFINITY + tof_offset)
			return centre + ir_offset;
		else
			return TOF_INFINITY + tof_offset;
	}
	else if (gauche == TOF_INFINITY)
	{// Cas � la con n�1
		if (centre == IR_INFINITY)
		{
			return droite + tof_offset;
		}
		else
		{
			return (centre + ir_offset + droite + tof_offset) / 2;
		}
	}
	else if (droite == TOF_INFINITY)
	{// Cas � la con n�2
		if (centre == IR_INFINITY)
		{
			return gauche + tof_offset;
		}
		else
		{
			return (centre + ir_offset + gauche + tof_offset) / 2;
		}
	}
	else
	{// Obstacle proche vu par tous les capteurs
		return ((gauche + droite) / 2) + tof_offset; // Le capteur central manque de pr�cision � cette distance
	}
}

float Robot::calculateFrontAngle(uint32_t gauche, uint32_t droite)
{
	return atan(((double)gauche - (double)droite) / 50);
}

void Robot::scriptCloseDoors(Side side)
{
	Position ici, noUncertainty(50, 30, 0);
	motionControlSystem.getPosition(ici);

	// Fermeture de la premi�re porte
	Trajectory closeFirstDoor;

	// Etape 1 : aller jusqu'� la premi�re porte
	UnitMove goToDoor;
	if (side == GREEN)
		goToDoor.setBendRadiusMm(130);
	else
		goToDoor.setBendRadiusMm(-150);
	goToDoor.setLengthMm(120);
	goToDoor.setSpeedMm_S(300);
	goToDoor.stopAfterMove = false;
	closeFirstDoor.push_back(goToDoor);


	goToDoor.setBendRadiusMm(INFINITE_RADIUS);
	goToDoor.setLengthMm(100);
	goToDoor.setSpeedMm_S(300);
	goToDoor.stopAfterMove = false;
	closeFirstDoor.push_back(goToDoor);

	// Etape 2 : pousser la premi�re porte
	UnitMove pushDoor;
	if (side == GREEN)
		pushDoor.setBendRadiusMm(-90);
	else
		pushDoor.setBendRadiusMm(90);
	pushDoor.setLengthMm(150);
	pushDoor.setSpeedMm_S(300);
	pushDoor.stopAfterMove = false;
	closeFirstDoor.push_back(pushDoor);

	// Fermeture de la premi�re porte
	table.enableUpdateObstacleMap(false);
	motionControlSystem.setTrajectory(closeFirstDoor);
	while (motionControlSystem.isMoving());

	
	// R�glage de la position (on vient de se recaler contre la porte)
	if (side == GREEN)
		ici.x = 1200;
	else
		ici.x = -1200;
	ici.y = 1937;
	ici.orientation = M_PI_2;
	motionControlSystem.setPosition(ici);
	motionControlSystem.setPositionUncertainty(noUncertainty);

	table.enableUpdateObstacleMap(true);

	Trajectory closeSecondDoor;

	// Aller de la premi�re � la seconde porte
	UnitMove disengage;
	disengage.setBendRadiusMm(INFINITE_RADIUS);
	disengage.setLengthMm(-10);
	disengage.setSpeedMm_S(300);
	disengage.stopAfterMove = false;
	closeSecondDoor.push_back(disengage);

	if (side == GREEN)
		goToDoor.setBendRadiusMm(145);
	else
		goToDoor.setBendRadiusMm(-130);
	goToDoor.setLengthMm(-470);
	goToDoor.setSpeedMm_S(300);
	goToDoor.stopAfterMove = false;
	closeSecondDoor.push_back(goToDoor);

	// Pousser la seconde porte
	pushDoor.setBendRadiusMm(INFINITE_RADIUS);
	pushDoor.setLengthMm(-100);
	pushDoor.setSpeedMm_S(300);
	pushDoor.stopAfterMove = false;
	closeSecondDoor.push_back(pushDoor);


	// Fermeture de la seconde porte
	table.enableUpdateObstacleMap(false);
	motionControlSystem.setTrajectory(closeSecondDoor);
	while (motionControlSystem.isMoving());

	// R�glage de la position (on vient de se recaler contre la porte)
	if (side == GREEN)
		ici.x = 900;
	else
		ici.x = -900;
	ici.y = 1937;
	ici.orientation = -M_PI_2;
	motionControlSystem.setPosition(ici);
	motionControlSystem.setPositionUncertainty(noUncertainty);
	
	table.enableUpdateObstacleMap(true);
}

void Robot::scriptGoToTowelFromDoors(Side side)
{
	// Aller de la seconde porte � la serviette
	Trajectory goToTowel;
	UnitMove unitMove;
	if (side == GREEN)
		unitMove.setBendRadiusMm(975);
	else
		unitMove.setBendRadiusMm(-875);
	unitMove.setLengthMm(1120);
	unitMove.setSpeedMm_S(300);
	unitMove.stopAfterMove = false;
	goToTowel.push_back(unitMove);


	// Pousser contre le bord de table pour bien se recaler
	unitMove.setBendRadiusMm(INFINITE_RADIUS);
	unitMove.setLengthMm(60);
	unitMove.setSpeedMm_S(350);
	unitMove.stopAfterMove = false;
	goToTowel.push_back(unitMove);


	table.enableUpdateObstacleMap(true);
	motionControlSystem.setTrajectory(goToTowel);
	while (motionControlSystem.isMoving())
	{
		// Lorsque le premier unitMove est suffisamment entam� (= on est proche du bord de table) on d�sactive l'�vitement
		if (motionControlSystem.getMoveProgress() > 0.4)
		{
			table.enableUpdateObstacleMap(false);
		}
	}

	Position ici;
	if (side == GREEN)
	{
		ici.x = 1437;
		ici.orientation = 0;
	}
	else
	{
		ici.x = -1437;
		ici.orientation = M_PI;
	}
	ici.y = 1100;
	Position uncertainty(80, 100, 0);
	motionControlSystem.setPosition(ici);
	motionControlSystem.setPositionUncertainty(uncertainty);

	table.enableUpdateObstacleMap(true);
}

void Robot::scriptPushSand(Side side)
{
	table.enableUpdateObstacleMap(true);
	table.enableLastChanceAvoiding(false);

	std::vector<Obstacle> movableVisible;
	RelativeObstacleMap allValues;
	int32_t frontDistance;
	Position destination;
	size_t bestObstacle;

	uint32_t timeout = 10000, beginTime = millis();

	do
	{
		// On met � jour notre objectif : l'obstacle 'MovableVisible' repr�sentant le tas de sable devant la serviette
		movableVisible = table.getMovableVisible();
		if (movableVisible.size() == 0)
		{
			return;
		}
		bestObstacle = 0;
		while (movableVisible.at(bestObstacle).getPriority() != KNOWN_SAND_PRIORITY)
		{
			bestObstacle++;
			if (bestObstacle > movableVisible.size())
			{
				return;
			}
		}

		movableVisible.at(bestObstacle).getCenter(destination);
		if (side == GREEN)
		{
			destination.x += movableVisible.at(bestObstacle).getXRadius();
		}
		else
		{
			destination.x -= movableVisible.at(bestObstacle).getXRadius();
		}

		goStraightToPoint(destination, 300);

		sensorMgr.getRelativeObstacleMapNoReset(allValues);
		frontDistance = calculateFrontDistance(allValues.arriereGauche, allValues.arriere, allValues.arriereDroit);

		delay(30);

	} while (frontDistance > CONTACT_OBSTACLE + 45 && millis() - beginTime < timeout);

	table.enableUpdateObstacleMap(false);

	if (side == GREEN)
	{
		destination.x = 350;
		destination.orientation = 0;
	}
	else
	{
		destination.x = -350;
		destination.orientation = M_PI;
	}
	destination.y = 1000;
	goStraightToPoint(destination, 200);

	while (motionControlSystem.isMoving());

	table.enableUpdateObstacleMap(true);
}

void Robot::scriptYolo(Side side, uint32_t duration)
{
	uint32_t beginTime = millis();

	table.enableUpdateObstacleMap(true);

	Position notrePosition;

	// Recalage contre le bord de table (plus ou moins au niveau de la serviette)
	Trajectory recalage;
	UnitMove unitMove;
	unitMove.setBendRadiusMm(INFINITE_RADIUS);
	unitMove.setLengthMm(1500);
	unitMove.setSpeedMm_S(350);
	unitMove.stopAfterMove = false;
	recalage.push_back(unitMove);

	motionControlSystem.setTrajectory(recalage);

	while (motionControlSystem.isMoving())
	{
		motionControlSystem.getPosition(notrePosition);
		if (notrePosition.x > 1200 || notrePosition.x < -1200)
		{
			table.enableUpdateObstacleMap(false);
		}

		if (millis() - beginTime >= duration)
		{
			motionControlSystem.stop();
			return;
		}
	}

	if (side == GREEN)
	{
		notrePosition.x = 1437;
		notrePosition.orientation = 0;
	}
	else
	{
		notrePosition.x = -1437;
		notrePosition.orientation = M_PI;
	}
	motionControlSystem.setPosition(notrePosition);
	Position uncertainty(40, 100, 0);
	motionControlSystem.setPositionUncertainty(uncertainty);
	// Fin du recalage

	// D�but de la trajectoire YOLO :D
	Trajectory yoloTrajectory;
	std::vector<Position> pointsDePassage;
	Position point;
	int mirror;
	if (side == GREEN)
	{
		mirror = 1;
	}
	else
	{
		mirror = -1;
	}

	point.x = mirror * 1000;
	point.y = 550;
	pointsDePassage.push_back(point);

	point.x = mirror * 1250;
	point.y = 750;
	pointsDePassage.push_back(point);

	point.x = mirror * 1250;
	point.y = 1200;
	pointsDePassage.push_back(point);

	point.x = mirror * 950;
	point.y = 200;
	pointsDePassage.push_back(point);

	point.x = mirror * 1250;
	point.y = 450;
	pointsDePassage.push_back(point);

	point.x = mirror * 1250;
	point.y = 1100;
	pointsDePassage.push_back(point);

	table.enableUpdateObstacleMap(true);

	for (size_t i = 0; i < pointsDePassage.size(); i++)
	{
		motionControlSystem.getPosition(notrePosition);
		yoloTrajectory = dummyPathFinding.computePath(notrePosition, pointsDePassage.at(i), 250, true);
		motionControlSystem.setTrajectory(yoloTrajectory);
		while (motionControlSystem.isMoving())
		{
			if (millis() - beginTime >= duration)
			{
				motionControlSystem.stop();
				return;
			}
		}
	}
}

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
	int value = 0;
	pinMode(PIN_DEL_ONBOARD, OUTPUT);

	while (value < 80)
	{// Tant que le jumper n'est pas inséré
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

void Robot::driveAlongEdgeOfTable(Side side, float kp, float ki, float kd)
{
	// Détermine la fréquence d'asservissement
	const uint32_t delaiAsservissement = 25; // Période d'asservissement, en ms
	uint32_t beginTime; // Permet le réglage de la période dans la boucle d'asservissement

	RelativeObstacleMap allValues; // Pour récupérer les valeurs des capteurs
	volatile int32_t sensorValue;	// Distance au bord de table mesurée, en mm
	volatile int32_t aimValue;		// Distance voulue, en mm
	volatile int32_t output = 0;	// Indique à quel point il faudra tourner.
	int32_t frontDistance;			// Distance au bord faisant face au robot, indiquant quand il faudra s'arrêter

	PID sensorPID(&sensorValue, &output, &aimValue); // PID pour asservissement sur la distance au bord de table;

	Trajectory trajectoireAsservie;
	UnitMove courbeAsservie;
	courbeAsservie.setLengthMm(100); // Longueur suffisante pour que le mouvement ne se termine pas entre deux asservissements de trajectoire
	courbeAsservie.setBendRadiusMm(INFINITE_RADIUS); // Valeur initiale, qui sera modifiée par le PID
	courbeAsservie.setSpeedMm_S(350); // Vitesse du mouvement, la vitesse la plus fiable et testée a été choisie
	courbeAsservie.stopAfterMove = false; // Inutile de s'arrêter entre les mouvements
	trajectoireAsservie.push_back(courbeAsservie);

	/* La distance au bord doit être de 15cm, le capteur doit donc lire 115mm */
	aimValue = 115;

	/* Réglage des constantes d'asservissement */
	//sensorPID.setTunings(0.5, 0, 5);
	sensorPID.setTunings(kp, ki, kd); // DEBUG

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
		sensorPID.compute(); // Met à jour la valeur de 'output'
		
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

		frontDistance = calculateFrontDistance(allValues.avantGauche, allValues.avant, allValues.avantDroit);

		while (millis() - beginTime < delaiAsservissement);
	} while (frontDistance > 300);
	
	// Mouvement final (permettant l'arrêt)
	trajectoireAsservie.at(0).setBendRadiusMm(INFINITE_RADIUS);
	trajectoireAsservie.at(0).setLengthMm(30);
	trajectoireAsservie.at(0).stopAfterMove = true;
	motionControlSystem.setTrajectory(trajectoireAsservie);
	while (motionControlSystem.isMoving());
	
	delay(200);

	// Une fois le robot arrêté, on règle précisément la position à l'aide des capteurs
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
	
	// On règle également l'incertitude sur la position
	Position newUncertainty;
	newUncertainty.x = 0;
	newUncertainty.y = 0;
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
	{// Cas à la con n°1
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
	{// Cas à la con n°2
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
		return ((gauche + droite) / 2) + tof_offset; // Le capteur central manque de précision à cette distance
	}
}

float Robot::calculateFrontAngle(uint32_t gauche, uint32_t droite)
{
	return atan(((double)gauche - (double)droite) / 50);
}

void Robot::scriptCloseDoors(Side side)
{
	Position ici, noUncertainty(0, 0, 0);
	motionControlSystem.getPosition(ici);

	// Fermeture de la première porte
	Trajectory closeFirstDoor;

	// Etape 1 : aller jusqu'à la première porte
	UnitMove goToDoor;
	if (side == GREEN)
		goToDoor.setBendRadiusMm(120);
	else
		goToDoor.setBendRadiusMm(-120);
	goToDoor.setLengthMm(200);
	goToDoor.setSpeedMm_S(350);
	goToDoor.stopAfterMove = false;
	closeFirstDoor.push_back(goToDoor);

	// Etape 2 : fermer la première porte
	UnitMove pushDoor;
	if (side == GREEN)
		pushDoor.setBendRadiusMm(-100);
	else
		pushDoor.setBendRadiusMm(100);
	pushDoor.setLengthMm(250);
	pushDoor.setSpeedMm_S(350);
	pushDoor.stopAfterMove = false;
	closeFirstDoor.push_back(pushDoor);

	// Fermeture de la première porte
	motionControlSystem.setTrajectory(closeFirstDoor);
	while (motionControlSystem.isMoving());
	
	// Réglage de la position (on vient de se recaler contre la porte)
	if (side == GREEN)
		ici.x = 1200;
	else
		ici.x = -1200;
	ici.y = 1937;
	ici.orientation = M_PI_2;
	motionControlSystem.setPosition(ici);
	motionControlSystem.setPositionUncertainty(noUncertainty);


	// Etape 3 : se désengager de la première porte
	Trajectory closeSecondDoor;
	UnitMove disengage;
	if (side == GREEN)
		disengage.setBendRadiusMm(-120);
	else
		disengage.setBendRadiusMm(120);
	disengage.setLengthMm(-145);
	disengage.setSpeedMm_S(300);
	disengage.stopAfterMove = true;
	closeSecondDoor.push_back(disengage);

	// Etape 4 : aller jusqu'à la seconde porte
	if (side == GREEN)
		goToDoor.setBendRadiusMm(-700);
	else
		goToDoor.setBendRadiusMm(700);
	goToDoor.setLengthMm(300);
	goToDoor.setSpeedMm_S(350);
	goToDoor.stopAfterMove = false;
	closeSecondDoor.push_back(goToDoor);

	// Etape 5 : pousser la seconde porte
	if (side == GREEN)
		pushDoor.setBendRadiusMm(-100);
	else
		pushDoor.setBendRadiusMm(100);
	pushDoor.setLengthMm(250);
	pushDoor.setSpeedMm_S(350);
	pushDoor.stopAfterMove = false;
	closeSecondDoor.push_back(pushDoor);

	// Fermeture de la seconde porte
	motionControlSystem.setTrajectory(closeSecondDoor);
	while (motionControlSystem.isMoving());

	// Réglage de la position (on vient de se recaler contre la porte)
	if (side == GREEN)
		ici.x = 900;
	else
		ici.x = -900;
	ici.y = 1937;
	ici.orientation = M_PI_2;
	motionControlSystem.setPosition(ici);
	motionControlSystem.setPositionUncertainty(noUncertainty);
}

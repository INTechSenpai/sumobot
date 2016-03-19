#include "BattControler.h"
#include "SensorMgr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Motor.h"
#include "Path.h"
#include "Table.h"
#include "robot.h"
#include <vector>

#define INPUT_LENGH	64

char inputBuffer[INPUT_LENGH];

#define PIN_STARTUP_SIGNAL	A11 // Pin de l'interrupteur de démarrage du match
#define PIN_DEL_ONBOARD		13	// Pin de la DEL intégrée à la Teensy

MotionControlSystem motionControlSystem;
SensorMgr sensorMgr;
BattControler battControler;
IntervalTimer motionControlThread;
IntervalTimer sensorThread;

Table table(60,410,1000,1000);
bool robotPerdu;
Robot loliRobotKawaii;

Trajectory trajectory;
UnitMove unitMove;
RelativeObstacleMap obstacleMap;
Position ici;
ObstacleCercle robotAdverse;



uint8_t read(char* string) {
	static unsigned char buffer;
	uint8_t i = 0;

	do {
		while (!Serial.available())
		{

		}
		buffer = Serial.read();

		if (i == 0 && buffer == '\r') {
			return 0;
		}

		if (i == 0 && buffer == '\n') {
			continue;
		}

		string[i] = buffer;
		i++;
	} while (string[i - 1] != '\r');

	string[i - 1] = '\0';

	return 0;
}

void startupProcedure()
{
	// Indication de l'état "initialisation"
	pinMode(PIN_DEL_ONBOARD, OUTPUT);
	digitalWrite(PIN_DEL_ONBOARD, HIGH);

	uint32_t beginPressedTime = 0;
	bool pressed = false;
	uint32_t beginTime = millis();
	while (millis() - beginTime < 5000 || pressed)
	{
		if (analogRead(PIN_STARTUP_SIGNAL) < 500)
		{// Bouton relaché
			pressed = false;
		}
		else if (!pressed)
		{// Le bouton vient d'être pressé
			beginPressedTime = millis();
			pressed = true;
		}

		if (millis() - beginPressedTime > 3000 && pressed)
		{// Bouton appuyé depuis plus de 3 secondes
			pinMode(PIN_DEL_OK, OUTPUT);
			pinMode(PIN_DEL_BATT, OUTPUT);

			while (true) // On passe en mode "rangement des pelles" (et on y reste jusqu'au prochain redémarrage)
			{
				digitalWrite(PIN_DEL_BATT, HIGH);
				while (analogRead(PIN_STARTUP_SIGNAL) > 500); // On attend que le bouton soit relaché
				while (analogRead(PIN_STARTUP_SIGNAL) < 500); // On attend que le bouton soit pressé
				digitalWrite(PIN_DEL_BATT, LOW);
				digitalWrite(PIN_DEL_OK, HIGH);
				motionControlSystem.resetMove();
				digitalWrite(PIN_DEL_OK, LOW);
			}
		}
	}

	// Indication de la fin de l'initialisation
	digitalWrite(PIN_DEL_ONBOARD, LOW);
	pinMode(PIN_DEL_ONBOARD, INPUT);
}

void disengageProcedure(bool avG, bool avD, bool arG, bool arD)
{
	/* Marche arrière à effectuer */
	if ((avG && !avD) && (!arG && !arD))
	{
		motionControlSystem.desengageMove(false, false);
	}
	else if ((!avG && avD) && (!arG && !arD))
	{
		motionControlSystem.desengageMove(false, false);
	}
	else if ((avG && avD) && (!arG && !arD))
	{
		motionControlSystem.desengageMove(false, false);
	}

	/* Marche avant à effectuer */
	else if ((!avG && !avD) && (arG && !arD))
	{
		motionControlSystem.desengageMove(true, false);
	}
	else if ((!avG && !avD) && (!arG && arD))
	{
		motionControlSystem.desengageMove(true, false);
	}
	else if ((!avG && !avD) && (arG && arD))
	{
		motionControlSystem.desengageMove(true, false);
	}

	/* Dégagement courbe */
	else if ((avG && !avD) && (arG && !arD))
	{
		motionControlSystem.desengageMove(true, true);
	}
	else if ((!avG && avD) && (!arG && arD))
	{
		motionControlSystem.desengageMove(false, true);
	}

	/* Dans les autres cas, soit on ne sait pas se dégager, soit on est encore sur la table */
}


void setup()
{
	Serial.begin(9600);

	// Bullshit de debug (début)
		unitMove.setBendRadiusMm(375);
		unitMove.setLengthTicks(50000);
		unitMove.setSpeedTicks_S(4000);
		unitMove.stopAfterMove = true;

		trajectory.push_back(unitMove);

		ici.x = 0;
		ici.y = 0;
		ici.orientation = 0;

		motionControlSystem.setPosition(ici);
		motionControlSystem.setRightSpeedTunings(2, 0.01, 50);
		motionControlSystem.setLeftSpeedTunings(2, 0.01, 50);
	// Bullshit de debug (fin)

	Wire.begin();
	delay(50);

	sensorMgr.powerON();

	motionControlThread.priority(64);
	motionControlThread.begin(motionControlInterrupt, 500);

	//startupProcedure();

	sensorThread.priority(128);
	sensorThread.begin(sensorInterrupt, 30000);

	/*
	while (analogRead(PIN_STARTUP_SIGNAL) < 500); // On attend que le bouton soit pressé
	uint32_t t = millis();
	pinMode(PIN_DEL_ONBOARD, OUTPUT);
	while (millis() - t < 5000)
	{
		digitalWrite(PIN_DEL_ONBOARD, HIGH);
		delay(100);
		digitalWrite(PIN_DEL_ONBOARD, LOW);
		delay(100);
	}
	motionControlSystem.deployMove();
	*/
	// TODO : attendre 5 secondes en faisant clignotter la DEL orange intégrée et en déterminant notre position initiale
}

void loop()
{
	//motionControlSystem.deployMove();


	/*
	motionControlSystem.getPosition(ici);
	sensorMgr.getRelativeObstacleMap(obstacleMap);
	bool perdu;
	perdu = table.updateObstacleMap(obstacleMap, ici);
	motionControlSystem.setPosition(ici);


	if ((obstacleMap.solAvantGauche != 0 || obstacleMap.solAvantDroit != 0) || (obstacleMap.solArriereGauche != 0 || obstacleMap.solArriereDroit != 0))
	{
		Serial.printf("avG: %d | avD %d ||| arG: %d | arD: %d\n",
			obstacleMap.solAvantGauche,
			obstacleMap.solAvantDroit,
			obstacleMap.solArriereGauche,
			obstacleMap.solArriereDroit
			);
		Serial.println();
	}
	delay(100);
	//*/
	
	/*
	Serial.printf("x: %f | y: %f | o: %f | perdu: %d\n", ici.x, ici.y, ici.orientation, perdu);
	//*/

	//table.updateObstacleMap(obstacleMap, ici);
	//robotAdverse = table.getRobotAdverse();

	//*/

	/*
	Serial.printf("av: %d | avG: %d | avD %d ||| ar: %d | arG: %d | arD: %d ||| G: %d | D:%d\n",
		obstacleMap.avant,
		obstacleMap.avantGauche,
		obstacleMap.avantDroit,
		obstacleMap.arriere,
		obstacleMap.arriereGauche,
		obstacleMap.arriereDroit,
		obstacleMap.gauche,
		obstacleMap.droit
		);
	//*/
	/*
	Serial.printf("av: %d | avG: %d | avD %d ||| ar: %d | arG: %d | arD: %d ||| G: %d | D:%d\n",
		obstacleMap.speedAvant,
		obstacleMap.speedAvantGauche,
		obstacleMap.speedAvantDroit,
		obstacleMap.speedArriere,
		obstacleMap.speedArriereGauche,
		obstacleMap.speedArriereDroit,
		obstacleMap.speedGauche,
		obstacleMap.speedDroit
		);

	Serial.printf("x: %f | y: %f | o: %f | xs: %f | ys: %f\n",
		robotAdverse.position.x,
		robotAdverse.position.y,
		robotAdverse.position.orientation,
		robotAdverse.position.xSpeed,
		robotAdverse.position.ySpeed);
	//*/
	/*
	Serial.println();

	delay(100);
	//*/

	//*
	static uint32_t begin, end;

	begin = micros();
	motionControlSystem.getPosition(ici);
	sensorMgr.getRelativeObstacleMap(obstacleMap);
	robotPerdu = table.updateObstacleMap(obstacleMap, ici);

	motionControlSystem.setPosition(ici);
	loliRobotKawaii.strategy(table, robotPerdu, motionControlSystem.isMoving(), ici, trajectory);
	motionControlSystem.setTrajectory(trajectory);
	while (micros() - begin < 100000);

	//*/




	static float kp = 12, ki = 0, kd = 0;
	static int speed = 350, bendRadius = INFINITE_RADIUS, length = 200;

	if (Serial.available())
	{
		read(inputBuffer);
		if (!strcmp(inputBuffer, "a"))
		{
			Serial.println("Go");
			delay(3000);
			motionControlSystem.testAsservVitesse(speed, 20000, kp, ki, kd);
		}
		else if (!strcmp(inputBuffer, "speed"))
		{
			Serial.println("Speed ?");
			read(inputBuffer);
			speed = atoi(inputBuffer);
			Serial.print("Speed= ");
			Serial.println(speed);
		}
		else if (!strcmp(inputBuffer, "kp"))
		{
			Serial.println("Kp ?");
			read(inputBuffer);
			kp = atof(inputBuffer);
			Serial.printf("Kp= %g\n", kp);
		}
		else if (!strcmp(inputBuffer, "ki"))
		{
			Serial.println("Ki ?");
			read(inputBuffer);
			ki = atof(inputBuffer);
			Serial.printf("Ki= %g\n", ki);
		}
		else if (!strcmp(inputBuffer, "kd"))
		{
			Serial.println("Kd ?");
			read(inputBuffer);
			kd = atof(inputBuffer);
			Serial.printf("Kd= %g\n", kd);
		}
		else if (!strcmp(inputBuffer, "d"))
		{
			//*
			Serial.printf("Kp= %g\n", kp);
			Serial.printf("Ki= %g\n", ki);
			Serial.printf("Kd= %g\n", kd);
			Serial.printf("Speed= %d\n", speed);
			//*/
			Serial.printf("Length= %d\n", length);
			Serial.printf("BendR= %d\n", bendRadius);
		}
		else if (!strcmp(inputBuffer, "s"))
		{
			sensorMgr.getRelativeObstacleMap(obstacleMap);
			Serial.printf("Sol avant droit : %d\n", obstacleMap.solAvantDroit);
		}
		else if (!strcmp(inputBuffer, "t"))
		{
			motionControlSystem.testAsservVitesse(speed, 1000, kp, ki, kd);
			int32_t left, right;
			while (true) 
			{
				motionControlSystem.getCurrentSpeed(left, right);
				Serial.printf("g: %d | d: %d\n", left, right);
			}
		}
		else if (!strcmp(inputBuffer, "b"))
		{
			Serial.println("Bend radius ?");
			read(inputBuffer);
			if (!strcmp(inputBuffer, "inf"))
			{
				bendRadius = INFINITE_RADIUS;
			}
			else
			{
				bendRadius = atoi(inputBuffer);
			}
			Serial.printf("BendR= %d\n", bendRadius);
		}
		else if (!strcmp(inputBuffer, "l"))
		{
			Serial.println("Length ?");
			read(inputBuffer);
			length = atoi(inputBuffer);
			Serial.printf("Length= %d\n", length);
		}
		else if (!strcmp(inputBuffer, "z"))
		{
			Serial.println("Go !");
			trajectory[0].setBendRadiusMm(bendRadius);
			trajectory[0].setLengthMm(length);
			trajectory[0].setSpeedMm_S(speed);
			motionControlSystem.setRotationTunings(kp, ki, kd);
			motionControlSystem.setTrajectory(trajectory);
		}
		else if (!strcmp(inputBuffer, "xy"))
		{
			motionControlSystem.getPosition(ici);
			Serial.printf("x: %g | y: %g | o: %g\n", ici.x, ici.y, ici.orientation);
		}
		else if (!strcmp(inputBuffer, "rp"))
		{
			ici.x = 0;
			ici.y = 0;
			ici.orientation = 0;
			ici.xSpeed = 0;
			ici.ySpeed = 0;
			motionControlSystem.setPosition(ici);
		}
		else if (!strcmp(inputBuffer, "deploy"))
		{
			Serial.println("Deploy !");
			motionControlSystem.deployMove();
		}
		else if (!strcmp(inputBuffer, "reset"))
		{
			Serial.println("Reset !");
			motionControlSystem.resetMove();
		}

		Serial.println("");
	}
}



/* Fonction appellée toutes les 500µs réalisant l'asservissement */
void motionControlInterrupt()
{
	static int compteurTracker = 1;

	/* Asservissement du robot en vitesse et position */
	motionControlSystem.control();
	motionControlSystem.updatePosition();
	motionControlSystem.manageStop();


	if (compteurTracker == 4)
	{
		motionControlSystem.track();
		compteurTracker = 0;
	}
	compteurTracker++;
}


/* Fonction appellée toutes les 30ms mettant à jour les capteurs (à tour de rôle pour les capteurs lents) */
void sensorInterrupt()
{
	static RelativeObstacleMap floorObstacleMap; // Utilisé pour détecter les bords de table le plus bas niveau (rapidement) possible

	/* Mise à jour des DELs indiquant l'état de la batterie */
	battControler.control();

	// Mise à jour des capteurs du sol
	sensorMgr.updateFloor();

	static uint8_t compteur = 0;
	if (compteur == 0)
	{
		sensorMgr.updateFront();
		compteur++;
	}
	else if (compteur == 1)
	{
		sensorMgr.updateBack();
		compteur++;
	}
	else if (compteur == 2)
	{
		sensorMgr.updateSides();
		compteur = 0;
	}
	

	sensorMgr.getRelativeObstacleMapNoReset(floorObstacleMap);
	/* LIMITE_NB est défini dans table.h */
	disengageProcedure(
		floorObstacleMap.solAvantGauche < LIMITE_NB && floorObstacleMap.solAvantGauche != 0,
		floorObstacleMap.solAvantDroit < LIMITE_NB && floorObstacleMap.solAvantDroit != 0,
		floorObstacleMap.solArriereGauche < LIMITE_NB && floorObstacleMap.solArriereGauche != 0,
		floorObstacleMap.solArriereDroit < LIMITE_NB && floorObstacleMap.solArriereDroit != 0
		);
} 


/* Ce bout de code permet de compiler avec std::vector */
namespace std {
	void __throw_bad_alloc()
	{
		Serial.println("Unable to allocate memory");
	}

	void __throw_length_error(char const*e)
	{
		Serial.print("Length Error :");
		Serial.println(e);
	}
}
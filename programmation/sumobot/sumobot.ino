#include "BattControler.h"
#include "SensorMgr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Motor.h"
#include "Path.h"
#include "Table.h"
#include <vector>

#define INPUT_LENGH	64

char inputBuffer[INPUT_LENGH];


MotionControlSystem motionControlSystem;
SensorMgr sensorMgr;
BattControler battControler;
IntervalTimer motionControlThread;
IntervalTimer sensorThread;

Table table(60,400,1000,1000);

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



void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);

	unitMove.setBendRadiusMm(375);
	unitMove.setLengthTicks(50000);
	unitMove.setSpeedTicks_S(4000);
	unitMove.stopAfterMove = true;

	trajectory.push_back(unitMove);

	ici.x = 50;
	ici.y = 50;
	ici.orientation = PI / 4 - ANGLE_CAPTEUR_SOL;

	motionControlSystem.setPosition(ici);
	motionControlSystem.setRightSpeedTunings(2, 0.01, 50);
	motionControlSystem.setLeftSpeedTunings(2, 0.01, 50);

	Wire.begin();
	delay(50);

	sensorMgr.powerON();

	motionControlThread.priority(64);
	motionControlThread.begin(motionControlInterrupt, 500);

	sensorThread.priority(128);
	sensorThread.begin(sensorInterrupt, 30000);
}

void loop()
{
	/*
	ici = motionControlSystem.getPosition();
	sensorMgr.getRelativeObstacleMap(obstacleMap);
	bool perdu;
	perdu = table.updateObstacleMap(obstacleMap, ici);
	motionControlSystem.setPosition(ici);

	Serial.printf("avG: %d | avD %d ||| arG: %d | arD: %d\n",
		obstacleMap.solAvantGauche,
		obstacleMap.solAvantDroit,
		obstacleMap.solArriereGauche,
		obstacleMap.solArriereDroit
		);

	Serial.printf("x: %f | y: %f | o: %f | perdu: %d\n", ici.x, ici.y, ici.orientation, perdu);


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

	static float kp = 2, ki = 0.01, kd = 50;
	static int speed = 5000, bendRadius = 375, length = 300;

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
			/*
			Serial.printf("Kp= %g\n", kp);
			Serial.printf("Ki= %g\n", ki);
			Serial.printf("Kd= %g\n", kd);
			Serial.printf("Speed= %d\n", speed);
			*/
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
			motionControlSystem.setTrajectory(trajectory);
		}
		else if (!strcmp(inputBuffer, "xy"))
		{
			ici = motionControlSystem.getPosition();
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
	static uint32_t t1, t2;

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
	/* Mise à jour des DELs indiquant l'état de la batterie */
	battControler.control();

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
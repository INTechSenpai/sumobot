#include "MotionControlSystem.h"
#include "SensorMgr.h"

#ifndef _CUNIT_h
#define _CUNIT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define INPUT_BUFFER_LENGH	64

class CUnit
{
public:
	CUnit() : 
		motionControlSystem(MotionControlSystem::Instance()),
		sensorMgr(SensorMgr::Instance())
	{
	};


	/* 
	Pour faires des tests manuels : via l'interface série
	Notamment l'asservissement et les déplacements
	Méthode bloquante ! (boucle infinie)
	*/
	void serialInterface()
	{
		float kp = 1, ki = 0, kd = 0;
		int speed = 350, bendRadius = INFINITE_RADIUS, length = 200;

		Trajectory trajectory;
		UnitMove unitMove;
		Position ici;

		ici.x = 0;
		ici.y = 0;
		ici.orientation = 0;

		unitMove.setLengthMm(0);
		unitMove.setBendRadiusMm(0);
		unitMove.setSpeedMm_S(0);
		unitMove.stopAfterMove = true;
		trajectory.push_back(unitMove);

		while (true)
		{
			if (Serial.available())
			{
				read(inputBuffer);
				if (!strcmp(inputBuffer, "a"))
				{
					motionControlSystem.testAsservVitesse(speed, 1000, kp, ki, kd);
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

					Serial.printf("Kp= %g\n", kp);
					Serial.printf("Ki= %g\n", ki);
					Serial.printf("Kd= %g\n", kd);
					Serial.printf("Speed= %d\n", speed);
					Serial.printf("Length= %d\n", length);
					Serial.printf("BendR= %d\n", bendRadius);
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
	}


	/*
		Affiche les valeurs lues par les capteurs
		'displayOnlyRelevant' permet de n'afficher que les valeurs non nulles
	*/
	void sensors(bool displayFloor, bool displayProximity, bool displaySpped, bool displayOnlyRelevant = true)
	{
		RelativeObstacleMap obstacleMap;
		Position ici;
		motionControlSystem.getPosition(ici);
		sensorMgr.getRelativeObstacleMap(obstacleMap);


		/* Capteurs du sol */
		if (displayFloor)
		{
			if (
					!displayOnlyRelevant ||
					(
						(obstacleMap.solAvantGauche != 0 || obstacleMap.solAvantDroit != 0) ||
						(obstacleMap.solArriereGauche != 0 || obstacleMap.solArriereDroit != 0)
					)
				)
			{
				Serial.printf("avG: %d | avD %d ||| arG: %d | arD: %d\n",
					obstacleMap.solAvantGauche,
					obstacleMap.solAvantDroit,
					obstacleMap.solArriereGauche,
					obstacleMap.solArriereDroit
					);
			}
		}


		/* Capteurs de proximité */
		if (displayProximity)
		{
			if (
					!displayOnlyRelevant ||
					(
						(obstacleMap.avant != 0 || obstacleMap.avantGauche != 0) ||
						(obstacleMap.avantDroit != 0 || obstacleMap.arriere != 0) ||
						(obstacleMap.arriereGauche != 0 || obstacleMap.arriereDroit != 0) ||
						(obstacleMap.gauche != 0 || obstacleMap.droit != 0)
					)
				)
			{
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
			}
		}


		/* Vitesse de variation des capteurs de proximité */
		if (displaySpped)
		{
			if (
					!displayOnlyRelevant ||
					(
						(obstacleMap.speedAvant != 0 || obstacleMap.speedAvantGauche != 0) ||
						(obstacleMap.speedAvantDroit != 0 || obstacleMap.speedArriere != 0) ||
						(obstacleMap.speedArriereGauche != 0 || obstacleMap.speedArriereDroit != 0) ||
						(obstacleMap.speedGauche != 0 || obstacleMap.speedDroit != 0)
					)
				)
			{
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
			}
		}

		Serial.println();
		delay(100);
	}


private:
	MotionControlSystem & motionControlSystem;
	SensorMgr & sensorMgr;

	char inputBuffer[INPUT_BUFFER_LENGH];

	void read(char* string) {
		static unsigned char buffer;
		uint8_t i = 0;

		do {
			while (!Serial.available())
			{

			}
			buffer = Serial.read();

			if (i == 0 && buffer == '\r') {
				return;
			}

			if (i == 0 && buffer == '\n') {
				continue;
			}

			string[i] = buffer;
			i++;
		} while (string[i - 1] != '\r');

		string[i - 1] = '\0';

		return;
	}
};


#endif


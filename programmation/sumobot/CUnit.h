#ifndef _CUNIT_h
#define _CUNIT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MotionControlSystem.h"
#include "SensorMgr.h"
#include "pathfinding.h"


#define INPUT_BUFFER_LENGH	64

class CUnit
{
public:
	CUnit() : 
		motionControlSystem(MotionControlSystem::Instance()),
		sensorMgr(SensorMgr::Instance()),
		robot(Robot::Instance()),
		table(Table::Instance())
	{
	};


	/* 
	Pour faires des tests manuels : via l'interface série
	Notamment l'asservissement et les déplacements
	Méthode bloquante ! (boucle infinie)
	*/
	void serialInterface()
	{
		float kp = 2, ki = 0.01, kd = 50;
		int speed = 350, bendRadius = INFINITE_RADIUS;
		float length = 200;

		Pathfinding pathfinding;

		Trajectory trajectory;
		UnitMove unitMove;
		Position ici;
		Table table;
		ObstacleMap obstacleMap;
		table.initObstacleMap(GREEN);

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
					motionControlSystem.testAsservVitesseEtPosition(speed, (int)length, kp, ki, kd);
				}
				if (!strcmp(inputBuffer, "f"))
				{
					Serial.println(motionControlSystem.isMoving());
					Serial.println(motionControlSystem.isBlocked());
				}
				if (!strcmp(inputBuffer, "c"))
				{
					int32_t left, right;
					motionControlSystem.getTicks(left, right);
					Serial.printf("G: %d\tD: %d\n", left, right);
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
					Serial.printf("Length= %f\n", length);
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
					length = atof(inputBuffer);
					Serial.printf("Length= %f\n", length);
				}
				else if (!strcmp(inputBuffer, "z"))
				{
					Serial.println("Go !");
					trajectory[0].setBendRadiusMm(bendRadius);
					if (bendRadius == 0)
					{
						trajectory[0].setLengthRadians(length);
					}
					else
					{
						trajectory[0].setLengthMm( (int)length );
					}
					trajectory[0].setSpeedMm_S(speed);
					//motionControlSystem.setRotationTunings(kp, ki, kd);
					motionControlSystem.setTrajectory(trajectory);
				}
				else if (!strcmp(inputBuffer, "xy"))
				{
					motionControlSystem.getPosition(ici);
					Serial.printf("x: %g | y: %g | o: %g\n", ici.x, ici.y, ici.orientation);
					motionControlSystem.getPositionUncertainty(ici);
					Serial.printf("dx: %g | dy: %g | do: %g\n", ici.x, ici.y, ici.orientation);
					Serial.println();
				}
				else if (!strcmp(inputBuffer, "rp"))
				{
					ici.x = 0;
					ici.y = 0;
					ici.orientation = 0;
					motionControlSystem.setPosition(ici);
				}
				else if (!strcmp(inputBuffer, "traj"))
				{
					Trajectory testTrajectory;
					UnitMove unitTranslation;
					UnitMove unitRotation;
					
					unitTranslation.setBendRadiusMm(-400);
					unitTranslation.setLengthMm(400);
					unitTranslation.setSpeedMm_S(speed);
					unitTranslation.stopAfterMove = true;

					unitRotation.setBendRadiusMm(0);
					unitRotation.setLengthRadians( -M_PI / 2);
					unitRotation.setSpeedMm_S(speed);
					unitRotation.stopAfterMove = true;

					for (int i = 0; i < 4; i++)
					{
						testTrajectory.push_back(unitTranslation);
						testTrajectory.push_back(unitRotation);
					}

					motionControlSystem.setTrajectory(testTrajectory);

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
				else if (!strcmp(inputBuffer, "path"))
				{
					Serial.println("X ?");
					read(inputBuffer);
					int x = atoi(inputBuffer);
					Serial.print("X= ");
					Serial.println(x);

					Serial.println("Y ?");
					read(inputBuffer);
					int y = atoi(inputBuffer);
					Serial.print("Y= ");
					Serial.println(y);

					Serial.println("O ?");
					read(inputBuffer);
					float o = atof(inputBuffer);
					Serial.print("O= ");
					Serial.println(o);

					Position destination;
					destination.x = x;
					destination.y = y;
					destination.orientation = o;

					Position ici;
					ici.x = 1350;
					ici.y = 1150;
					ici.orientation = M_PI;
					Serial.printf("x=%g  y=%g  o=%g\n", ici.x, ici.y, ici.orientation);
					
					obstacleMap = table.getObstacleMap();

					Serial.printf("fixed invisible: %d\n", obstacleMap.fixedInvisible.size());
					Serial.printf("fixed visible: %d\n", obstacleMap.fixedVisible.size());
					Serial.printf("movable invisible: %d\n", obstacleMap.movableInvisible.size());
					Serial.printf("movable visible: %d\n", obstacleMap.movableVisible.size());
					Serial.printf("oponent robot: %d\n", obstacleMap.oponentRobot.size());
					Serial.printf("To be specified: %d\n", obstacleMap.toBeSpecified.size());

					Trajectory trajectory = pathfinding.computePath(obstacleMap, ici, destination);

					Serial.printf("Trajectory : %d", trajectory.size());

					motionControlSystem.setTrajectory(trajectory);

				}
				else if (!strcmp(inputBuffer, "slide"))
				{
					int color;
					do 
					{
						Serial.println("Side? (0:Vert | 1:Violet)\n");
						read(inputBuffer);
						color = atoi(inputBuffer);
					} while (color != 0 && color != 1);
					Side side;
					if (color == 0)
					{
						side = GREEN;
						Serial.println("VERT");
					}
					else
					{
						side = PURPLE;
						Serial.println("VIOLET");
					}

					Serial.println("kp ?");
					read(inputBuffer);
					float kp = atof(inputBuffer);
					Serial.printf("kp= %g\n", kp);

					Serial.println("ki ?");
					read(inputBuffer);
					float ki = atof(inputBuffer);
					Serial.printf("ki= %g\n", ki);

					Serial.println("kd ?");
					read(inputBuffer);
					float kd = atof(inputBuffer);
					Serial.printf("kd= %g\n", kd);

					robot.driveAlongEdgeOfTable(side, kp, ki, kd);
					motionControlSystem.getPosition(ici);
					Serial.printf("x: %g | y: %g | o: %g\n", ici.x, ici.y, ici.orientation);
				}
				else if (!strcmp(inputBuffer, "cd"))
				{
					Serial.println("Test script close door");
					robot.driveAlongEdgeOfTable(PURPLE, 0.5, 0, 5);
					robot.scriptCloseDoors(PURPLE);
					Serial.println("Done");
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

	void obstacleDetection()
	{
		RelativeObstacleMap relativeObstacleMap;
		Table::DetectionPoint detectionPoint[NB_CAPTEURS];
		Position ici;
		ici.x = 1350;
		ici.y = 1150;
		ici.orientation = M_PI_2;
		Position incertitude;
		incertitude.x = 50;
		incertitude.y = 50;
		incertitude.orientation = 0.2;
		
		while (true)
		{
			sensorMgr.getRelativeObstacleMapNoReset(relativeObstacleMap);
			table.fillDetectionPoints(detectionPoint, ici, relativeObstacleMap);
			table.interpreteDetectionPoints(detectionPoint, ici, incertitude);
			table.moveRobotToMatchFixedObstacles(detectionPoint, ici);
			table.moveObstaclesToMatchDetection(detectionPoint);
			table.deleteUndetectedObstacles(detectionPoint, ici);
			table.addObstaclesToBeDeterminated(detectionPoint, ici);
			table.deleteOutdatedObstacles();
			

			for (int i = 0; i < NB_CAPTEURS; i++)
			{
				if (detectionPoint[i].isAnObstacle)
				{
					Serial.printf("(%d) x: %g | y: %g\t[%d]\t", i, detectionPoint[i].x - ici.x, detectionPoint[i].y - ici.y, table.getToBeSpecifiedLength());
					switch (detectionPoint[i].associatedObstacleType)
					{
					case MOVABLE_VISIBLE:
						Serial.println("MOVABLE_VISIBLE");
						break;
					case MOVABLE_INVISIBLE:
						Serial.println("MOVABLE_INVISIBLE");
						break;
					case FIXED_VISIBLE:
						Serial.println("FIXED_VISIBLE");
						break;
					case FIXED_INVISIBLE:
						Serial.println("FIXED_INVISIBLE");
						break;
					case TO_BE_SPECIFIED:
						Serial.println("TO_BE_SPECIFIED");
						break;
					case OPONENT_ROBOT:
						Serial.println("OPONENT_ROBOT");
						break;
					case NONE:
						Serial.println("NONE");
						break;
					default:
						break;
					}
				}
			}
			Serial.println();
			delay(100);
		}
	}

	void obstacleCreationDeletion()
	{
		Position ici;
		ici.x = -920;
		ici.y = 1400;
		ici.orientation = M_PI_4;
		motionControlSystem.setPosition(ici);
		ObstacleMap obstacleMap;
		obstacleMap = table.getObstacleMap();

		Position centreObstacle;
		Serial.println("Begin");
		for (int i = 0; i < obstacleMap.toBeSpecified.size(); i++)
		{
			obstacleMap.toBeSpecified.at(i).getCenter(centreObstacle);
			Serial.printf("#O# x:%g  y:%g  o:%g  r:%g  ttl:%d  lts:%d\n", 
				centreObstacle.x, centreObstacle.y, centreObstacle.orientation, 
				obstacleMap.toBeSpecified.at(i).getRadius(),
				obstacleMap.toBeSpecified.at(i).getTTL(),
				obstacleMap.toBeSpecified.at(i).getLastTimeSeen());
		}
		delay(100);
	}

private:
	MotionControlSystem & motionControlSystem;
	SensorMgr & sensorMgr;
	Robot & robot;
	Table & table;

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


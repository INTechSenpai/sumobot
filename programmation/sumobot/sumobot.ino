#include "BattControler.h"
#include "SensorMgr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Motor.h"
#include "Path.h"
#include <vector>

#define INPUT_LENGH	64

char inputBuffer[INPUT_LENGH];


MotionControlSystem motionControlSystem;
SensorMgr sensorMgr;
BattControler battControler;
IntervalTimer motionControlThread;
IntervalTimer sensorThread;


Trajectory trajectory;
UnitMove unitMove;
RelativeObstacleMap obstacleMap;
Position ici;



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

	unitMove.setBendRadiusTicks(INFINITE_RADIUS);
	unitMove.setLengthTicks(2000);
	unitMove.setSpeedTicks_S(1000);

	trajectory.push_back(unitMove);

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
	
	sensorMgr.getRelativeObstacleMap(obstacleMap);
	//Serial.println(obstacleMap.avantGauche);
	//Serial.println(obstacleMap.avantDroit);
	//Serial.println(obstacleMap.arriereGauche);
	//Serial.println(obstacleMap.arriereDroit);
	Serial.println(obstacleMap.speedGauche);
	//Serial.println(obstacleMap.droit);
	//Serial.println(obstacleMap.avant);
	//Serial.println(obstacleMap.arriere);

	Serial.println();
	delay(100);
	


	static float kp = 2, ki = 0.01, kd = 50;
	static int speed = 5000;

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
		}
		else if (!strcmp(inputBuffer, "s"))
		{
			sensorMgr.getRelativeObstacleMap(obstacleMap);
			Serial.printf("Sol avant droit : %d\n", obstacleMap.solAvantDroit);
		}
		Serial.println("");
	}
}



/* Fonction appell�e toutes les 500�s r�alisant l'asservissement et indiquant le niveau de batterie */
void motionControlInterrupt()
{
	/* Mise � jour des DELs indiquant l'�tat de la batterie */
	battControler.control();

	/* Asservissement du robot en vitesse et position */
	motionControlSystem.control();
	motionControlSystem.updatePosition();
	motionControlSystem.manageStop();
}


/* Fonction appell�e toutes les 30ms mettant � jour les capteurs (� tour de r�le pour les capteurs lents) */
void sensorInterrupt()
{
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
#include "BattControler.h"
//#include "SensorMgr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Motor.h"
#include "Path.h"
#include <vector>

#define INPUT_LENGH	64

char inputBuffer[INPUT_LENGH];

MotionControlSystem motionControlSystem;
IntervalTimer lowLevelThread;

Trajectory trajectory;
UnitMove unitMove;

volatile uint32_t endTime;
uint32_t avG, avD, arG, arD;

//SensorMgr sensorMgr;
//RelativeObstacleMap obstacleMap;

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

	//Wire.begin();
	delay(50);

	endTime = 0;

	attachInterrupt(25, interrupt, FALLING);

	//sensorMgr.powerON();

	//ColorSensor::init(24, 25, 26, 27);

	//lowLevelThread.priority(128);
	//lowLevelThread.begin(lowLevelInterrupt, 500);
}

void interrupt()
{
	endTime = micros();
}

void loop()
{
	uint32_t beginTime;
	while (true)
	{
		pinMode(25, OUTPUT);
		digitalWrite(25, HIGH);
		delayMicroseconds(10);
		pinMode(25, INPUT);
		digitalWrite(25, LOW);
		beginTime = micros();

		delay(30);

		Serial.println(beginTime);
		Serial.println(endTime);
	}




	/*
	ColorSensor::update();
	delay(10);
	ColorSensor::read(avG, avD, arG, arD, true);
	Serial.printf("avG %d, avD %d, arG %d, arD %d\n", avG, avD, arG, arD);
	*/

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
			//obstacleMap = sensorMgr.getRelativeObstacleMap();
			//Serial.printf("Sol avant droit : %d\n", obstacleMap.solAvantDroit);
		}
		Serial.println("");
	}
}

void lowLevelInterrupt()
{
	static BattControler battControler;
	battControler.control();

	//sensorMgr.updateObstacleMap();
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
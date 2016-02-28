#include "ToFSensor.h"
#include "SensorMgr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Motor.h"
#include "Path.h"
#include <vector>

MotionControlSystem motionControlSystem;

Trajectory trajectory;
UnitMove unitMove;

VL6180X capteurToF;

uint8_t gpio0 = 4;

ToFSensor tofSensor(42, gpio0);

void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	pinMode(29, OUTPUT);
	pinMode(30, OUTPUT);
	digitalWrite(13, HIGH);

	unitMove.bendRadius = INFINITE_RADIUS;
	unitMove.length = 5000;
	unitMove.speed = 8000;

	trajectory.push_back(unitMove);
	/*
	motionControlSystem.enablePositionControl(false);
	motionControlSystem.enablePwmControl(false);
	//*/

	Wire.begin();

	tofSensor.powerON();

}

void loop()
{
	delay(1000);
	digitalWrite(30, HIGH);


	Serial.println("go");

	Serial.println(tofSensor.getMesure());



	/*
	motionControlSystem.setRawPWM(1023, 1023);
	for (int i = 0; i < 5; i++)
	{
		motionControlSystem.control();
		motionControlSystem.track();
		delayMicroseconds(500);
	}

	motionControlSystem.printTracking();
	//*/
/*
	motionControlSystem.setTrajectory(trajectory);
	while (motionControlSystem.isMoving())
	{

		motionControlSystem.control();
		motionControlSystem.manageStop();
		delayMicroseconds(500);
	}
	Serial.println(motionControlSystem.isBlocked());
//*/
	//Serial.println(analogRead(A10));
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
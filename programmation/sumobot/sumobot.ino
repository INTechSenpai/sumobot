#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Path.h"
#include <vector>


Trajectory trajectory;
UnitMove unitMove;


MotionControlSystem motionControlSystem;


void setup()
{
	Serial.begin(9600);
	Serial.println("C'est parti !");
	pinMode(13, OUTPUT);
	pinMode(29, OUTPUT);
	pinMode(30, OUTPUT);
	digitalWrite(13, HIGH);
}

void loop()
{
	digitalWrite(13, LOW);
	digitalWrite(30, HIGH);
	Serial.println("C'est parti !");

	unitMove.bendRadius = INFINITE_RADIUS;
	unitMove.length = 5000;
	unitMove.length = 600;

	delay(1000);
	digitalWrite(13, HIGH);
	digitalWrite(30, LOW);
	Serial.println("C'est parti !");
	motionControlSystem.setTrajectory(trajectory);
	while (motionControlSystem.isMoving())
	{
		motionControlSystem.control();
		motionControlSystem.manageStop();
		delayMicroseconds(500);
	}
	Serial.println("done.");

	delay(1000);

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
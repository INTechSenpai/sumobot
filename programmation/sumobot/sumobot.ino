#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Motor.h"
#include "Path.h"
#include <vector>

Motor motor;

void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	pinMode(29, OUTPUT);
	pinMode(30, OUTPUT);
	digitalWrite(13, HIGH);
}

void loop()
{
	delay(1000);
	digitalWrite(30, HIGH);

	motor.runLeft(850);
	motor.runRight(1023);

	Serial.println(analogRead(A10));
	

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
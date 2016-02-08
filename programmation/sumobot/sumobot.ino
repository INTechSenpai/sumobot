#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"


void setup()
{
	pinMode(13, OUTPUT);
  /* add setup code here */
}

void loop()
{
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	delay(500);
  /* add main program code here */

}

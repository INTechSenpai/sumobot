#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Motor
{
public:
	Motor()
	{
		pinEnableLeft = 1;
		pinEnableRight = 0;
		pinPwmLeft = 21;
		pinPwmRight = 23;
		pinDirectionLeft = 20;
		pinDirectionRight = 22;

		pinMode(pinEnableLeft, OUTPUT);
		pinMode(pinEnableRight, OUTPUT);
		pinMode(pinPwmLeft, OUTPUT);
		pinMode(pinPwmRight, OUTPUT);
		pinMode(pinDirectionLeft, OUTPUT);
		pinMode(pinDirectionRight, OUTPUT);


		// La résolution des PWM est 10bits (0-1023)
		analogWriteResolution(10);

		// Réglage de la fréquence des PWM
		analogWriteFrequency(pinPwmLeft, 35156.25);
		analogWriteFrequency(pinPwmRight, 35156.25);

		// Initialisation : Moteurs activés mais arrêtés
		analogWrite(pinPwmLeft, 0);
		analogWrite(pinPwmRight, 0);
		digitalWrite(pinDirectionLeft, LOW);
		digitalWrite(pinDirectionRight, LOW);

		enableLeft(true);
		enableRight(true);
	}

	void runLeft(int16_t pwm)
	{
		if (pwm >= 0)
		{
			digitalWrite(pinDirectionLeft, HIGH);
			if (pwm > 1023)
				pwm = 1023;
			pwm = 1023 - pwm;
		}
		else
		{
			digitalWrite(pinDirectionLeft, LOW);
			pwm = -pwm;
			if (pwm > 1023)
				pwm = 1023;
		}
		analogWrite(pinPwmLeft, pwm);
	}

	void runRight(int16_t pwm)
	{
		if (pwm >= 0)
		{
			digitalWrite(pinDirectionRight, LOW);
			if (pwm > 1023)
				pwm = 1023;
		}
		else
		{
			digitalWrite(pinDirectionRight, HIGH);
			pwm = -pwm;
			if (pwm > 1023)
				pwm = 1023;
			pwm = 1023 - pwm;
		}
		
		analogWrite(pinPwmRight, pwm);
	}

	void enableLeft(bool enable)
	{
		digitalWrite(pinEnableLeft, enable);
	}

	void enableRight(bool enable)
	{
		digitalWrite(pinEnableRight, enable);
	}

private:

	uint8_t
		pinEnableLeft,
		pinEnableRight,
		pinPwmLeft,
		pinPwmRight,
		pinDirectionLeft,
		pinDirectionRight;

};

#endif
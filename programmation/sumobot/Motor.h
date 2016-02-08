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
		pinEnableLeft = 0;
		pinEnableRight = 1;
		pinPwmLeft = 23;
		pinPwmRight = 21;
		pinDirectionLeft = 22;
		pinDirectionRight = 20;

		pinMode(pinEnableLeft, OUTPUT);
		pinMode(pinEnableRight, OUTPUT);
		pinMode(pinPwmLeft, OUTPUT);
		pinMode(pinPwmRight, OUTPUT);
		pinMode(pinDirectionLeft, OUTPUT);
		pinMode(pinDirectionRight, OUTPUT);

		// La résolution des PWM est 8bits (0-255)
		analogWriteResolution(8);

		// Réglage de la fréquence des PWM
		analogWriteFrequency(pinPwmLeft, 140625);
		analogWriteFrequency(pinPwmRight, 140625);

		// Initialisation : Moteurs activés mais arrêtés
		analogWrite(pinPwmLeft, 0);
		analogWrite(pinPwmRight, 0);

		enableLeft(true);
		enableRight(true);
	}

	void runLeft(int16_t pwm)
	{
		if (pwm >= 0)
		{
			digitalWrite(pinDirectionLeft, HIGH);
		}
		else
		{
			digitalWrite(pinDirectionLeft, LOW);
			pwm = -pwm;
		}
		if (pwm > 255)
			pwm = 255;
		analogWrite(pinPwmLeft, pwm);
	}

	void runRight(int16_t pwm)
	{
		if (pwm >= 0)
		{
			digitalWrite(pinDirectionRight, HIGH);
		}
		else
		{
			digitalWrite(pinDirectionRight, LOW);
			pwm = -pwm;
		}
		if (pwm > 255)
			pwm = 255;
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
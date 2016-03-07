#ifndef _COLORSENSOR_h
#define _COLORSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ProximitySensor.h"
#include "utils.h"

class ColorSensor
{
public:
	ColorSensor()
	{}

	static void init(uint8_t newPinAVg, uint8_t newPinAVd, uint8_t newPinARg, uint8_t newPinARd)
	{
		pinAVg = newPinAVg;
		pinAVd = newPinAVd;
		pinARg = newPinARg;
		pinARd = newPinARd;

		beginTime = 0;
		endTimeAVg = 0;
		endTimeAVd = 0;
		endTimeARg = 0;
		endTimeARd = 0;
		
		attachInterrupt(pinAVg, av_g_interrupt, FALLING);
		attachInterrupt(pinAVd, av_d_interrupt, FALLING);
		attachInterrupt(pinARg, ar_g_interrupt, FALLING);
		attachInterrupt(pinARd, ar_d_interrupt, FALLING);

		minTime = 1;
		maxTime = 3000;
	}

	/* Fonction de mise à jour des données des capteurs. Déclenche une lecture sur chaque capteur. S'exécute en 10µs environ */
	static void update()
	{
		pinMode(pinAVg, OUTPUT);
		pinMode(pinAVd, OUTPUT);
		pinMode(pinARg, OUTPUT);
		pinMode(pinARd, OUTPUT);
		digitalWrite(pinAVg, HIGH);
		digitalWrite(pinAVd, HIGH);
		digitalWrite(pinARg, HIGH);
		digitalWrite(pinARd, HIGH);

		delayMicroseconds(10);

		digitalWrite(pinAVg, LOW);
		digitalWrite(pinAVd, LOW);
		digitalWrite(pinARg, LOW);
		digitalWrite(pinARd, LOW);
		pinMode(pinAVg, INPUT);
		pinMode(pinAVd, INPUT);
		pinMode(pinARg, INPUT);
		pinMode(pinARd, INPUT);

		beginTime = micros();
	}

	/* 
	Renvoie les valeurs lues sur chaque capteur (une fois la lecture terminée). Si la valeur a déjà été lue la focntions renvoie 0
	Si le paramètre rawReading est à true, alors les valeurs retournées sont directement les temps mesurés 
	Sinon (cas par défaut), les valeurs retournées sont comprises entre 0 et 1000, sachant que
	1 correspond au temps minimal (blanc très blanc)
	1000 correspond au temps maximal (noir très noir)
	0 indique que la lecture n'est pas terminée ou bien que la valeur a déjà été lue.
	*/
	static void read(uint32_t & avG, uint32_t & avD, uint32_t & arG, uint32_t & arD, bool rawReading = false)
	{
		static uint32_t endAVg, endAVd, endARg, endARd;
		cli();
		endAVg = endTimeAVg;
		endAVd = endTimeAVd;
		endARg = endTimeARg;
		endARd = endTimeARd;
		endTimeAVg = 0;
		endTimeAVd = 0;
		endTimeARg = 0;
		endTimeARd = 0;
		sei();

		if (beginTime < endAVg)
		{
			avG = endAVg - beginTime;
			if (!rawReading)
			{
				avG = CONSTRAIN(avG, minTime, maxTime);
				avG = map(avG, minTime, maxTime, 1, 1000);
			}
		}
		else
		{
			avG = 0;
		}

		if (beginTime < endAVd)
		{
			avD = endAVd - beginTime;
			if (!rawReading)
			{
				avD = CONSTRAIN(avD, minTime, maxTime);
				avD = map(avD, minTime, maxTime, 1, 1000);
			}
		}
		else
		{
			avD = 0;
		}

		if (beginTime < endARg)
		{
			arG = endARg - beginTime;
			if (!rawReading)
			{
				arG = CONSTRAIN(arG, minTime, maxTime);
				arG = map(arG, minTime, maxTime, 1, 1000);
			}
		}
		else
		{
			arG = 0;
		}

		if (beginTime < endARd)
		{
			arD = endARd - beginTime;
			if (!rawReading)
			{
				arD = CONSTRAIN(arD, minTime, maxTime);
				arD = map(arD, minTime, maxTime, 1, 1000);
			}
		}
		else
		{
			arD = 0;
		}
	}

	static void calibrateBlack()
	{
		uint32_t newMaxTime = 0;
		uint32_t avG, avD, arG, arD;

		for (int i = 0; i < 50; i++)
		{
			update();
			delay(10);
			read(avG, avD, arG, arD, true);
			if (avG > newMaxTime)
				newMaxTime = avG;
			if (avD > newMaxTime)
				newMaxTime = avD;
			if (arG > newMaxTime)
				newMaxTime = arG;
			if (arD > newMaxTime)
				newMaxTime = arD;
		}
		maxTime = newMaxTime;
	}

	static void calibrateWhite()
	{
		uint32_t newMinTime = UINT32_MAX;
		uint32_t avG, avD, arG, arD;

		for (int i = 0; i < 50; i++)
		{
			update();
			delay(10);
			read(avG, avD, arG, arD, true);
			if (avG < newMinTime)
				newMinTime = avG;
			if (avD < newMinTime)
				newMinTime = avD;
			if (arG < newMinTime)
				newMinTime = arG;
			if (arD < newMinTime)
				newMinTime = arD;
		}
		minTime = newMinTime;
	}


private:
	static volatile uint32_t endTimeAVg, endTimeAVd, endTimeARg, endTimeARd;

	static void av_g_interrupt()
	{
		endTimeAVg = micros();
	}

	static void av_d_interrupt()
	{
		endTimeAVd = micros();
	}

	static void ar_g_interrupt()
	{
		endTimeARg = micros();
	}

	static void ar_d_interrupt()
	{
		endTimeARd = micros();
	}

	static uint8_t pinAVg, pinAVd, pinARg, pinARd;

	static uint32_t beginTime;

	/* Durée minimale et maximale entre beginTime et endTime. Le minimum correspond au blanc le plus blanc. Le maximum au noir le plus noir. */
	static uint32_t maxTime, minTime;

};

#endif


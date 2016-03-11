#ifndef _BATTCONTROLER_h
#define _BATTCONTROLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define PIN_SENSE_BATT	A10
#define PIN_DEL_BATT	29
#define PIN_DEL_OK		30

#define ADC_TO_VOLT		0.008
#define OFFSET_READ		0.5116

// Tensions exprimées en Volt
/* 
	Tension nominale LiPo : 7,4V 
	Tension maximale (ABSOLUE) : 8,5V
	Tension minimale (ABSOLUE) : 5,9V
*/

#define VOLTAGE_WARNING		7.1		// En dessous de cette tension, il faut penser à recharger la batterie
#define VOLTAGE_DANGER		6.6		// En dessous de cette tension, il est impératif de regarger la batterie
#define VOLTAGE_UNPLUGGED	4		// En dessous de cette tension, la batterie est débranchée

#define REFRESH_PERIODE	2000	// Delai entre deux lectures analogiques de la tension (en ms)

class BattControler
{
public:
	BattControler()
	{
		pinMode(PIN_DEL_OK, OUTPUT);
		digitalWrite(PIN_DEL_OK, LOW);
		pinMode(PIN_DEL_BATT, OUTPUT);
		digitalWrite(PIN_DEL_BATT, LOW);
		currentVoltage = 0;
	}

	inline void control()
	{
		static uint32_t lastRefresh = 0;
		if (millis() - lastRefresh >= REFRESH_PERIODE)
		{
			currentVoltage = readBatt();
			lastRefresh = millis();
		}

		if (currentVoltage <= VOLTAGE_WARNING && currentVoltage > VOLTAGE_DANGER)
		{ // Zone d'avertissement
			blinkLED(200, 800);
			digitalWrite(PIN_DEL_OK, LOW);
		}
		else if (currentVoltage <= VOLTAGE_DANGER && currentVoltage > VOLTAGE_UNPLUGGED)
		{ // Zone de danger
			blinkLED(100, 100);
			digitalWrite(PIN_DEL_OK, LOW);
		}
		else
		{ // Zone de sécurité
			blinkLED(0, 1000);
			digitalWrite(PIN_DEL_OK, HIGH);
		}
	}

	float getVoltage()
	{
		return currentVoltage;
	}

private:
	float readBatt()
	{
		return (float)analogRead(PIN_SENSE_BATT) * ADC_TO_VOLT + OFFSET_READ;
	}

	// Fait clignotter la LED de batterie faible avec les delais ON/OFF indiqués en ms
	void blinkLED(unsigned int periodeON, unsigned int periodeOFF)
	{
		static uint32_t lastChange = 0;
		static bool ledON = false;

		if (ledON)
		{
			if (millis() - lastChange >= periodeON && periodeOFF != 0)
			{
				digitalWrite(PIN_DEL_BATT, LOW);
				ledON = false;
				lastChange = millis();
			}
		}
		else
		{
			if (millis() - lastChange >= periodeOFF && periodeON != 0)
			{
				digitalWrite(PIN_DEL_BATT, HIGH);
				ledON = true;
				lastChange = millis();
			}
		}
	}


	float currentVoltage;
};

#endif


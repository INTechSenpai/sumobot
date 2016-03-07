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
	ColorSensor();

	static void init(uint8_t newPinAVg, uint8_t newPinAVd, uint8_t newPinARg, uint8_t newPinARd);

	/* Fonction de mise � jour des donn�es des capteurs. D�clenche une lecture sur chaque capteur. S'ex�cute en 10�s environ */
	static void update();

	/* 
	Renvoie les valeurs lues sur chaque capteur (une fois la lecture termin�e). Si la valeur a d�j� �t� lue la focntions renvoie 0
	Si le param�tre rawReading est � true, alors les valeurs retourn�es sont directement les temps mesur�s 
	Sinon (cas par d�faut), les valeurs retourn�es sont comprises entre 0 et 1000, sachant que
	1 correspond au temps minimal (blanc tr�s blanc)
	1000 correspond au temps maximal (noir tr�s noir)
	0 indique que la lecture n'est pas termin�e ou bien que la valeur a d�j� �t� lue.
	*/
	static void read(uint32_t & avG, uint32_t & avD, uint32_t & arG, uint32_t & arD, bool rawReading = false);

	static void calibrateBlack();

	static void calibrateWhite();

private:
	static volatile uint32_t endTimeAVg, endTimeAVd, endTimeARg, endTimeARd;
	
	static void av_g_interrupt();
	static void av_d_interrupt();
	static void ar_g_interrupt();
	static void ar_d_interrupt();

	static uint8_t pinAVg, pinAVd, pinARg, pinARd;

	static uint32_t beginTime;

	/* Dur�e minimale et maximale entre beginTime et endTime. Le minimum correspond au blanc le plus blanc. Le maximum au noir le plus noir. */
	static uint32_t maxTime, minTime;

};

#endif


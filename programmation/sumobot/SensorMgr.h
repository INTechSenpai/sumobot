#ifndef _SENSORMGR_h
#define _SENSORMGR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ToFSensor.h"
#include "InfraredSensor.h"
#include "ColorSensor.h"

/* PIN MAPPING */
/* Pin standby des capteurs ToF */

#define STBY_AV_GAUCHE	2
#define STBY_AV_DROIT	4
#define STBY_GAUCHE		6
#define STBY_DROIT		8
#define STBY_AR_GAUCHE	10
#define STBY_AR_DROIT	12


class SensorMgr
{
public:
	SensorMgr() : 
		capteurAvantGauche(42, STBY_AV_GAUCHE),
		capteurAvantDroit(43, STBY_AV_DROIT),
		capteurGauche(44, STBY_GAUCHE),
		capteurDroit(45, STBY_DROIT),
		capteurArriereGauche(46, STBY_AR_GAUCHE),
		capteurArriereDroit(47, STBY_AR_DROIT),
		capteurAvant(),
		capteurArriere(),
		solAvantGauche(),
		solAvantDroit(),
		solArriereGauche(),
		solArriereDroit()
	{
	}

	/* Allume tous les capteurs */
	void powerON()
	{
		capteurAvantGauche.powerON();
		capteurAvantDroit.powerON();
		capteurGauche.powerON();
		capteurDroit.powerON();
		capteurArriereGauche.powerON();
		capteurArriereDroit.powerON();
	}

	/* Eteint tous les capteurs */
	void powerOFF()
	{
		capteurAvantGauche.standby();
		capteurAvantDroit.standby();
		capteurGauche.standby();
		capteurDroit.standby();
		capteurArriereGauche.standby();
		capteurArriereDroit.standby();
	}

private:
	ToFSensor capteurAvantDroit;
	ToFSensor capteurAvantGauche;
	ToFSensor capteurDroit;
	ToFSensor capteurGauche;
	ToFSensor capteurArriereDroit;
	ToFSensor capteurArriereGauche;
	InfraredSensor capteurAvant;
	InfraredSensor capteurArriere;
	ColorSensor solAvantGauche;
	ColorSensor solAvantDroit;
	ColorSensor solArriereGauche;
	ColorSensor solArriereDroit;
};

#endif


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
#include "RelativeObstacleMap.h"

/* PIN MAPPING */
/* Pin standby des capteurs ToF */
#define STBY_AV_GAUCHE	2
#define STBY_AV_DROIT	4
#define STBY_GAUCHE		6
#define STBY_DROIT		8
#define STBY_AR_GAUCHE	10
#define STBY_AR_DROIT	12

/* Pin de commande des capteurs de couleur */
#define COLOR_AV_GAUCHE	24
#define COLOR_AV_DROIT	25
#define COLOR_AR_GAUCHE	26
#define COLOR_AR_DROIT	27

/* ### Durée de l'attente entre deux mesures pour chaque type de capteur (en µs) ### */
#define PERIODE_TOF	20000
#define PERIODE_IR	50000
#define PERIODE_SOL	20000


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
		capteurAvant(0x00),
		capteurArriere(0x80)
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

		capteurAvant.init();
		capteurArriere.init();

		ColorSensor::init(COLOR_AV_GAUCHE, COLOR_AV_DROIT, COLOR_AR_GAUCHE, COLOR_AR_DROIT);
	}

	/* Eteint tous les capteurs (sauf ceux qui ne s'éteignent pas) */
	void powerOFF()
	{
		capteurAvantGauche.standby();
		capteurAvantDroit.standby();
		capteurGauche.standby();
		capteurDroit.standby();
		capteurArriereGauche.standby();
		capteurArriereDroit.standby();
	}


	/* 
		Mise à jour de l'ensemble des capteurs
		Il est nécéssaire d'appeller cette méthode toutes les 500µs
		Ainsi les données des capteurs sont mises à jour à la vitesse optimale
	*/
	void updateObstacleMap()
	{
		static uint32_t tofLastUpdate = 0, irLastUpdate = 0, solLastUpdate = 0, now;
		now = micros();
		if (now - tofLastUpdate >= PERIODE_TOF && false)
		{
			relativeObstacleMap.avantGauche = capteurAvantGauche.getDistance();
			relativeObstacleMap.avantDroit = capteurAvantDroit.getDistance();
			relativeObstacleMap.gauche = capteurGauche.getDistance();
			relativeObstacleMap.droit = capteurDroit.getDistance();
			relativeObstacleMap.arriereGauche = capteurArriereGauche.getDistance();
			relativeObstacleMap.arriereDroit = capteurArriereDroit.getDistance();
			tofLastUpdate = now;
		}
		if (now - irLastUpdate >= PERIODE_IR && false)
		{
			relativeObstacleMap.avant = capteurAvant.getDistance();
			relativeObstacleMap.arriere = capteurArriere.getDistance();
			irLastUpdate = now;
		}
		if (now - solLastUpdate >= PERIODE_SOL)
		{
			ColorSensor::read(
				relativeObstacleMap.solAvantGauche, 
				relativeObstacleMap.solAvantDroit, 
				relativeObstacleMap.solArriereGauche, 
				relativeObstacleMap.solArriereDroit,
				true
				);
			ColorSensor::update();
			solLastUpdate = now;
		}
	}


	/* Ne force pas la mise à jour des capteurs, renvoie les valeurs précédement mesurées */
	RelativeObstacleMap & getRelativeObstacleMap()
	{
		static RelativeObstacleMap buffer;
		cli();
		buffer = relativeObstacleMap;
		relativeObstacleMap.arriere = 0;
		relativeObstacleMap.arriereDroit = 0;
		relativeObstacleMap.arriereGauche = 0;
		relativeObstacleMap.avant = 0;
		relativeObstacleMap.avantDroit = 0;
		relativeObstacleMap.avantGauche = 0;
		relativeObstacleMap.droit = 0;
		relativeObstacleMap.gauche = 0;
		relativeObstacleMap.solArriereDroit = 0;
		relativeObstacleMap.solArriereGauche = 0;
		relativeObstacleMap.solAvantDroit = 0;
		relativeObstacleMap.solAvantGauche = 0;
		sei();
		return buffer;
	}


private:
	ToFSensor capteurAvantGauche;
	ToFSensor capteurAvantDroit;
	ToFSensor capteurGauche;
	ToFSensor capteurDroit;
	ToFSensor capteurArriereGauche;
	ToFSensor capteurArriereDroit;
	InfraredSensor capteurAvant;
	InfraredSensor capteurArriere;

	RelativeObstacleMap relativeObstacleMap;
};

#endif


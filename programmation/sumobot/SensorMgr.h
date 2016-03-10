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

/* Pins de commande des capteurs de couleur */
#define C_POW_AV_GAUCHE	24
#define C_POW_AV_DROIT	25
#define C_POW_AR_GAUCHE	26
#define C_POW_AR_DROIT	27

/* Pins de lecture des capteurs de couleur*/
#define C_INT_AV_GAUCHE	33
#define C_INT_AV_DROIT	32
#define C_INT_AR_GAUCHE	31
#define C_INT_AR_DROIT	11

/* Fréquences de raffraichissement des capteurs de proximité, en Hz (pour le calcul de la vitesse de variation) */
#define FREQ_TOF	11
#define FREQ_IR		11


/* ### Durée de l'attente entre deux mesures pour chaque type de capteur (en µs) ### */
#define PERIODE_TOF	30000
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

		previousObstacleMap.arriere = 0;
		previousObstacleMap.arriereDroit = 0;
		previousObstacleMap.arriereGauche = 0;
		previousObstacleMap.avant = 0;
		previousObstacleMap.avantDroit = 0;
		previousObstacleMap.avantGauche = 0;
		previousObstacleMap.droit = 0;
		previousObstacleMap.gauche = 0;
		previousObstacleMap.solArriereDroit = 0;
		previousObstacleMap.solArriereGauche = 0;
		previousObstacleMap.solAvantDroit = 0;
		previousObstacleMap.solAvantGauche = 0;
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

		ColorSensor::init(
			C_POW_AV_GAUCHE,
			C_INT_AV_GAUCHE,
			C_POW_AV_DROIT,
			C_INT_AV_DROIT,
			C_POW_AR_GAUCHE,
			C_INT_AR_GAUCHE,
			C_POW_AR_DROIT,
			C_INT_AR_DROIT
			);
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


	/* Mise à jour des trois capteurs frontaux (durée de la maj : ~4ms) */
	void updateFront()
	{
		/* AVANT CENTRE */
		relativeObstacleMap.avant = capteurAvant.getDistance();
		relativeObstacleMap.speedAvant = (relativeObstacleMap.avant - previousObstacleMap.avant)*FREQ_IR;
		previousObstacleMap.avant = relativeObstacleMap.avant;

		/* AVANT GAUCHE */
		relativeObstacleMap.avantGauche = capteurAvantGauche.getDistance();
		relativeObstacleMap.speedAvantGauche = (relativeObstacleMap.avantGauche - previousObstacleMap.avantGauche)*FREQ_TOF;
		previousObstacleMap.avantGauche = relativeObstacleMap.avantGauche;

		/* AVANT DROIT */
		relativeObstacleMap.avantDroit = capteurAvantDroit.getDistance();
		relativeObstacleMap.speedAvantDroit = (relativeObstacleMap.avantDroit - previousObstacleMap.avantDroit)*FREQ_TOF;
		previousObstacleMap.avantDroit = relativeObstacleMap.avantDroit;
	}


	/* Mise à jour des trois capteurs arrière (durée de la maj : ~4ms) */
	void updateBack()
	{
		/* ARRIERE CENTRE */
		relativeObstacleMap.arriere = capteurArriere.getDistance();
		relativeObstacleMap.speedArriere = (relativeObstacleMap.arriere - previousObstacleMap.arriere)*FREQ_IR;
		previousObstacleMap.arriere = relativeObstacleMap.arriere;

		/* ARRIERE GAUCHE */
		relativeObstacleMap.arriereGauche = capteurArriereGauche.getDistance();
		relativeObstacleMap.speedArriereGauche = (relativeObstacleMap.arriereGauche - previousObstacleMap.arriereGauche)*FREQ_TOF;
		previousObstacleMap.arriereGauche = relativeObstacleMap.arriereGauche;

		/* ARRIERE DROIT */
		relativeObstacleMap.arriereDroit = capteurArriereDroit.getDistance();
		relativeObstacleMap.speedArriereDroit = (relativeObstacleMap.arriereDroit - previousObstacleMap.arriereDroit)*FREQ_TOF;
		previousObstacleMap.arriereDroit = relativeObstacleMap.arriereDroit;
	}


	/* Mise à jour des deux capteurs des côtés (durée de la maj : ~3,5ms) */
	void updateSides()
	{
		/* GAUCHE */
		relativeObstacleMap.gauche = capteurGauche.getDistance();
		relativeObstacleMap.speedGauche = (relativeObstacleMap.gauche - previousObstacleMap.gauche)*FREQ_TOF;
		previousObstacleMap.gauche = relativeObstacleMap.gauche;

		/* DROIT */
		relativeObstacleMap.droit = capteurDroit.getDistance();
		relativeObstacleMap.speedDroit = (relativeObstacleMap.droit - previousObstacleMap.droit)*FREQ_TOF;
		previousObstacleMap.droit = relativeObstacleMap.droit;
	}


	/* Mise à jour des capteurs de couleur dirigés vers le sol (durée de la maj : ~10µs) */
	void updateFloor()
	{
		ColorSensor::read(
			relativeObstacleMap.solAvantGauche,
			relativeObstacleMap.solAvantDroit,
			relativeObstacleMap.solArriereGauche,
			relativeObstacleMap.solArriereDroit
			);
		ColorSensor::update();
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
		if (now - tofLastUpdate >= PERIODE_TOF)
		{
			relativeObstacleMap.avantGauche = capteurAvantGauche.getDistance();
			relativeObstacleMap.speedAvantGauche = relativeObstacleMap.avantGauche - previousObstacleMap.avantGauche;
			previousObstacleMap.avantGauche = relativeObstacleMap.avantGauche;

			relativeObstacleMap.avantDroit = capteurAvantDroit.getDistance();
			relativeObstacleMap.speedAvantDroit = relativeObstacleMap.avantDroit - previousObstacleMap.avantDroit;
			previousObstacleMap.avantDroit = relativeObstacleMap.avantDroit;

			relativeObstacleMap.gauche = capteurGauche.getDistance();
			relativeObstacleMap.speedGauche = relativeObstacleMap.gauche - previousObstacleMap.gauche;
			previousObstacleMap.gauche = relativeObstacleMap.gauche;

			relativeObstacleMap.droit = capteurDroit.getDistance();
			relativeObstacleMap.speedDroit = relativeObstacleMap.droit - previousObstacleMap.droit;
			previousObstacleMap.droit = relativeObstacleMap.droit;

			relativeObstacleMap.arriereGauche = capteurArriereGauche.getDistance();
			relativeObstacleMap.speedArriereGauche = relativeObstacleMap.arriereGauche - previousObstacleMap.arriereGauche;
			previousObstacleMap.arriereGauche = relativeObstacleMap.arriereGauche;

			relativeObstacleMap.arriereDroit = capteurArriereDroit.getDistance();
			relativeObstacleMap.speedArriereDroit = relativeObstacleMap.arriereDroit - previousObstacleMap.arriereDroit;
			previousObstacleMap.arriereDroit = relativeObstacleMap.arriereDroit;

			tofLastUpdate = now;
		}

		if (now - irLastUpdate >= PERIODE_IR)
		{
			relativeObstacleMap.avant = capteurAvant.getDistance();
			relativeObstacleMap.speedAvant = relativeObstacleMap.avant - previousObstacleMap.avant;
			previousObstacleMap.avant = relativeObstacleMap.avant;

			relativeObstacleMap.arriere = capteurArriere.getDistance();
			relativeObstacleMap.speedArriere = relativeObstacleMap.arriere - previousObstacleMap.arriere;
			previousObstacleMap.arriere = relativeObstacleMap.arriere;

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
	void getRelativeObstacleMap(RelativeObstacleMap & output)
	{
		cli(); // Désactivation des interruptions car elles modifient les valeurs de "relativeObstacleMap"
		output = relativeObstacleMap;
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
		sei(); // Réactivation des interruptions
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
	RelativeObstacleMap previousObstacleMap;
};

#endif


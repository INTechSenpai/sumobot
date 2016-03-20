#include "CUnit.h"
#include "BattControler.h"
#include "SensorMgr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Motor.h"
#include "Path.h"
#include "Table.h"
#include <vector>


#define PIN_STARTUP_SIGNAL	A11 // Pin de l'interrupteur de démarrage du match
#define PIN_DEL_ONBOARD		13	// Pin de la DEL intégrée à la Teensy



void setup()
{
	Serial.begin(9600);
	Wire.begin();
	delay(50);
}


void loop()
{
	MotionControlSystem & motionControlSystem = MotionControlSystem::Instance();
	SensorMgr & sensorMgr = SensorMgr::Instance();
	IntervalTimer motionControlThread;
	IntervalTimer sensorThread;

	sensorMgr.powerON();

	motionControlThread.priority(64);
	motionControlThread.begin(motionControlInterrupt, 500);

	sensorThread.priority(128);
	sensorThread.begin(sensorInterrupt, 30000);

	
}



/* Fonction appellée toutes les 500µs réalisant l'asservissement */
void motionControlInterrupt()
{
	static MotionControlSystem & motionControlSystem = MotionControlSystem::Instance();

	static int compteurTracker = 1;

	/* Asservissement du robot en vitesse et position */
	motionControlSystem.control();
	motionControlSystem.updatePosition();
	motionControlSystem.manageStop();


	if (compteurTracker == 4)
	{
		motionControlSystem.track();
		compteurTracker = 0;
	}
	compteurTracker++;
}


/* Fonction appellée toutes les 30ms mettant à jour les capteurs (à tour de rôle pour les capteurs lents) */
void sensorInterrupt()
{
	static SensorMgr & sensorMgr = SensorMgr::Instance();
	static BattControler battControler;


	/* Mise à jour des DELs indiquant l'état de la batterie */
	battControler.control();

	// Mise à jour des capteurs du sol
	sensorMgr.updateFloor();

	static uint8_t compteur = 0;
	if (compteur == 0)
	{
		sensorMgr.updateFront();
		compteur++;
	}
	else if (compteur == 1)
	{
		sensorMgr.updateBack();
		compteur++;
	}
	else if (compteur == 2)
	{
		sensorMgr.updateSides();
		compteur = 0;
	}
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
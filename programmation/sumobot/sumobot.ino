#include "Robot.h"
#include "Table.h"
#include "CUnit.h"
#include "BattControler.h"
#include "SensorMgr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "MotionControlSystem.h"
#include "Motor.h"
#include "Path.h"
#include "Obstacle.h"
#include <vector>


#define PIN_STARTUP_SIGNAL	A11 // Pin de l'interrupteur de d�marrage du match
#define PIN_DEL_ONBOARD		13	// Pin de la DEL int�gr�e � la Teensy



void setup()
{
	Serial.begin(9600);
	Wire.begin();
	Wire.setClock(1500000);
	delay(50);
}


void loop()
{
	Robot & robot = Robot::Instance();
	SensorMgr & sensorMgr = SensorMgr::Instance();
	IntervalTimer motionControlThread;
	IntervalTimer sensorThread;

	CUnit test;

	sensorMgr.powerON();

	motionControlThread.priority(64);
	motionControlThread.begin(motionControlInterrupt, 500);

	/*
	Side side = robot.checkSide();
	robot.init(side);
	//*/

	sensorThread.priority(128);
	sensorThread.begin(sensorInterrupt, 30000);

	/*
	robot.waitForBegining();
	robot.winMatch(90000);
	delay(2000);
	robot.deployUmbrella();
	//*/

	while (true)
	{
		test.sensors(false, true, false, false);
	}
}



/* Fonction appell�e toutes les 500�s r�alisant l'asservissement */
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


/* Fonction appell�e toutes les 30ms mettant � jour les capteurs (� tour de r�le pour les capteurs lents) */
void sensorInterrupt()
{
	static SensorMgr & sensorMgr = SensorMgr::Instance();
	static Table & table = Table::Instance();
	static BattControler battControler;


	/* Mise � jour des DELs indiquant l'�tat de la batterie */
	battControler.control();

	/* Les capteurs de couleur du sol ne sont pour l'instant plus utilis�s
	// Mise � jour des capteurs du sol
	sensorMgr.updateFloor();
	*/

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
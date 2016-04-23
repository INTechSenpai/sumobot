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
#include "pathfinding.h"


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
	IntervalTimer battControlerThread;

	CUnit test;

	sensorMgr.powerON();

	motionControlThread.priority(64);
	motionControlThread.begin(motionControlInterrupt, 500);

	sensorThread.priority(128);
	sensorThread.begin(sensorInterrupt, 25000);

	/*
	Side side = robot.checkSide();
	//*/

	battControlerThread.priority(80);
	battControlerThread.begin(battControlerInterrupt, 50000);

	//robot.waitForBegining();
	/*
	robot.winMatch(90000);
	delay(2000);
	robot.deployUmbrella();
	//*/


	test.serialInterface();

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
	static MotionControlSystem & motionControlSystem = MotionControlSystem::Instance();
	static Position robotPosition;
	static Position robotPositionUncertainty;
	static Table & table = Table::Instance();
	static RelativeObstacleMap relativeObstacleMap;

	sensorMgr.updateFront();
	sensorMgr.updateBack();
	sensorMgr.updateSides();
	
	sensorMgr.getRelativeObstacleMapNoReset(relativeObstacleMap);
	motionControlSystem.getPosition(robotPosition);
	motionControlSystem.getPositionUncertainty(robotPositionUncertainty);
	if (table.updateObstacleMap(relativeObstacleMap, robotPosition, robotPositionUncertainty))
	{
		motionControlSystem.setPosition(robotPosition);
		motionControlSystem.setPositionUncertainty(robotPositionUncertainty);
	}
}


/* Mise � jour des DELs indiquant l'�tat de la batterie */
void battControlerInterrupt()
{
	static BattControler battControler;
	battControler.control();
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

	void __throw_out_of_range(char const*e)
	{
		Serial.print("Out of range :");
		Serial.println(e);
	}
}
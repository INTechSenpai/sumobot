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

	Side side = robot.checkSide();
	robot.init(side);

	battControlerThread.priority(80);
	battControlerThread.begin(battControlerInterrupt, 50000);

	//robot.waitForBegining();
	robot.driveAlongEdgeOfTable(side, 0.5, 0, 5);
	robot.scriptCloseDoors(side);
	//robot.winMatch(90000);
	//delay(2000);
	//robot.deployUmbrella();


	//test.serialInterface();
	while (true)
	{
		//test.obstacleCreationDeletion();
		//test.obstacleDetection();
		//test.sensors(false, true, false, false);
	}
}



/* Fonction appellée toutes les 500µs réalisant l'asservissement */
void motionControlInterrupt()
{
	static MotionControlSystem & motionControlSystem = MotionControlSystem::Instance();
	static int compteurUpdatePosition = 1;
	static int compteurTracker = 1;

	/* Asservissement du robot en vitesse et position */
	motionControlSystem.control();
	if (compteurUpdatePosition == 10)
	{
		motionControlSystem.updatePosition();
		compteurUpdatePosition = 0;
	}
	compteurUpdatePosition++;
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
	// Evitement
	bool pause = !table.isTrajectoryAllowed(motionControlSystem.getTrajectory(), motionControlSystem.getCurrentMove());
	if (pause)
	{
		Serial.println("STOP YOU FOOL");
	}
	motionControlSystem.setPause(pause);
}


/* Mise à jour des DELs indiquant l'état de la batterie */
void battControlerInterrupt()
{
	static BattControler battControler;
	battControler.control();

	// DEBUG
	/*
	static MotionControlSystem & motionControlSystem = MotionControlSystem::Instance();
	static Position ici;
	motionControlSystem.getPosition(ici);
	Serial.printf("%g\t%g\t%g\n", ici.x, ici.y, ici.orientation);
	//*/
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
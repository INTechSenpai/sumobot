#include "Robot.h"

Robot::Robot() : 
	table(Table::Instance()),
	sensorMgr(SensorMgr::Instance()),
	motionControlSystem(MotionControlSystem::Instance())
{
}

void Robot::init(Side side)
{
	table.initObstacleMap(side);
	// TODO : régler la position initiale du robot (en fonction du côté)
	//motionControlSystem.setPosition();
}

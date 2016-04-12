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
	// TODO : r�gler la position initiale du robot (en fonction du c�t�)
	//motionControlSystem.setPosition();
}

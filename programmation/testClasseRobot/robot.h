#ifndef ROBOT_H
#define ROBOT_H

#include "../testPathfindingSimple/pathfinding.h"
#include "../sumobot/MotionControlSystem.h"
#include "../sumobot/Position.h"
#include "../sumobot/SensorMgr.h"
#include "../sumobot/Table.h"
#include "../sumobot/Path.h"


class Robot
{
public:
    Robot();
    void strategy();

private:
    Pathfinding pathfinding;

    MotionControlSystem motionControlSystem;
};

#endif // ROBOT_H

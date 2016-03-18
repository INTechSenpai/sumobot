#ifndef ROBOT_H
#define ROBOT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define MARGE_DETECTION_COTE 0.3
#define MARGE_TRAJECTOIRE 30

#include "pathfinding.h"
#include "MotionControlSystem.h"
#include "Position.h"
#include "SensorMgr.h"
#include "Table.h"
#include "Path.h"


class Robot
{
public:
    Robot();
    void strategy(Table &, bool, bool, Position &, Trajectory &);

private:
    Pathfinding pathfinding;
};

#endif // ROBOT_H

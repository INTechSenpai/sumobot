TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    robot.cpp \
    ../sumobot/MotionControlSystem.cpp \
    ../testPathfindingSimple/pathfinding.cpp

HEADERS += \
    robot.h \
    ../sumobot/MotionControlSystem.h \
    ../sumobot/Position.h \
    ../sumobot/SensorMgr.h \
    ../testPathfindingSimple/pathfinding.h

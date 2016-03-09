TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    mapdeuxdes.cpp \
    pathfinding.cpp \
    ../sumobot/Obstacle.cpp \
    ../sumobot/ObstacleCercle.cpp

HEADERS += \
    mapdeuxdes.h \
    pathfinding.h \
    ../sumobot/Obstacle.h \
    ../sumobot/ObstacleCercle.h \
    ../sumobot/ObstacleRectangle.h \
    ../sumobot/Position.h \
    ../sumobot/Path.h

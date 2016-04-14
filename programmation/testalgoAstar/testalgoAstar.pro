TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    pathfinding.cpp \
    ../sumobot/Obstacle.cpp

HEADERS += \
    pathfinding.h \
    ../sumobot/Path.h \
    ../sumobot/Obstacle.h \
    ../sumobot/Position.h \
    ../sumobot/Table.h \
    ../sumobot/WProgram.h

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    pathfinding.cpp

HEADERS += \
    pathfinding.h \
    ../sumobot/Position.h \
    ../sumobot/Path.h \
    ../sumobot/Obstacle.h

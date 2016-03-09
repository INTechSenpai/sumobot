TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    mapdeuxdes.cpp \
    pathfinding.cpp \
    ../TestMapping/ConsoleApplication1/ConsoleApplication1/Table.cpp \
    ../TestMapping/ConsoleApplication1/ConsoleApplication1/testmapping.cpp

HEADERS += \
    mapdeuxdes.h \
    pathfinding.h \
    ../sumobot/Path.h \
    ../TestMapping/ConsoleApplication1/ConsoleApplication1/Obstacle.h \
    ../TestMapping/ConsoleApplication1/ConsoleApplication1/Position.h \
    ../TestMapping/ConsoleApplication1/ConsoleApplication1/Table.h

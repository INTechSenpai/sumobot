#ifndef _DUMMYPATHFINDING_h
#define _DUMMYPATHFINDING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Path.h"
#include "Position.h"
#include "math.h"
#include "utils.h"

class DummyPathFinding
{
public:
	DummyPathFinding();

	Trajectory computePath(const Position & depart, const Position & destination, int32_t speed, bool stopAfterMove);

private:
	void calculateUnitMove(int32_t & bendRadius, int32_t & moveLength, const Position & depart, const Position & destination);
};

#endif


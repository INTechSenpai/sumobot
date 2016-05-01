#include "dummyPathFinding.h"

DummyPathFinding::DummyPathFinding()
{
}

Trajectory DummyPathFinding::computePath(const Position & depart, const Position & destination, int32_t speed, bool stopAfterMove)
{
	Trajectory trajectory;
	UnitMove unitMove;
	int32_t bendRadius, moveLength;

	calculateUnitMove(bendRadius, moveLength, depart, destination);

	unitMove.setBendRadiusMm(bendRadius);
	unitMove.setLengthMm(moveLength);
	unitMove.setSpeedMm_S(speed);
	unitMove.stopAfterMove = stopAfterMove; 
	trajectory.push_back(unitMove);

	return trajectory;
}

void DummyPathFinding::calculateUnitMove(int32_t & bendRadius, int32_t & moveLength,const Position & depart, const Position & destination)
{
	float angleObjectif = atan2(destination.y - depart.y, destination.x - depart.x);
	float distanceObjectif = sqrt(square(destination.y - depart.y) + square(destination.x - depart.x));
	float orientationDepart = depart.orientation;
	float theta = fmodulo(orientationDepart + M_PI_2 - angleObjectif, 2 * M_PI);
	float cosTheta = cos(theta);
	float angleTotal = angleObjectif - orientationDepart;

	if (ABS(cosTheta) > 0.0001)
	{
		bendRadius = distanceObjectif / (2 * cosTheta);

		if (angleTotal > M_PI_2)
		{
			angleTotal -= M_PI;
		}
		else if (angleTotal < -M_PI_2)
		{
			angleTotal += M_PI;
		}
		moveLength = 2 * bendRadius * angleTotal;
	}
	else
	{
		bendRadius = INFINITE_RADIUS;
		if (theta < PI)
		{
			moveLength = distanceObjectif;
		}
		else
		{
			moveLength = -distanceObjectif;
		}
	}

}

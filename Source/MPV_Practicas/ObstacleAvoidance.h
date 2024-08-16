#pragma once
#include "SteeringBase.h"

class ObstacleAvoidance : public SteeringBase
{
public:
	virtual FVector GetSteering(AAICharacter* _pChar, float DeltaTime);

private:
	
	FVector nearestPoint;
	FVector lookAhead;
	FVector neededPosition;
};

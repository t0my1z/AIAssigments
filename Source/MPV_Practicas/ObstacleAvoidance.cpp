#include "ObstacleAvoidance.h"

#include "AICharacter.h"
#include "debug/debugdraw.h"
#include "Engine/TargetPoint.h"
#include "Kismet/KismetMathLibrary.h"

FVector ObstacleAvoidance::GetSteering(AAICharacter* _pChar, float DeltaTime)
{
	TArray<FVector> ObstaclePoints;
	TArray<float> RadiusOfPoints;
	FObstacles& Obstacles = _pChar->m_pathObstacles;
	for (ATargetPoint* TargetPoint : Obstacles.locations)
	{
		ObstaclePoints.Add(TargetPoint->GetActorLocation());
	}
	for (float& ObstacleRadius : Obstacles.radiuses)
	{
		RadiusOfPoints.Add(ObstacleRadius);
	}
	
	float TimeAhead = _pChar->GetParams().time_ahead;

	FVector PredictedPos = _pChar->GetParams().targetPosition;
	FVector CalculatedAvoidPosition = FVector::Zero();
	for (int i = 0; i < ObstaclePoints.Num(); ++i)
	{
		FVector dirToObstacle = ObstaclePoints[i] - _pChar->GetActorLocation();
		FVector dirToPredictedPos = PredictedPos - _pChar->GetActorLocation();
		dirToPredictedPos.Normalize();

		float distToObstacle = FVector::DotProduct(dirToPredictedPos, dirToObstacle);

		FVector PlayerLookAhead = dirToPredictedPos * distToObstacle;
		PlayerLookAhead += _pChar->GetActorLocation();

		float distToObstacleFromLookAhead = FVector::Dist(PlayerLookAhead, ObstaclePoints[i]);
		
		if(distToObstacleFromLookAhead < RadiusOfPoints[i] + _pChar->GetParams().radius)
		{
			CalculatedAvoidPosition = PlayerLookAhead - ObstaclePoints[i];
		}
	}

	if(CalculatedAvoidPosition == FVector::Zero())
	{
		return PredictedPos;
	}
	
	return CalculatedAvoidPosition;
}

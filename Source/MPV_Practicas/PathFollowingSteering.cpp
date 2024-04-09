// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFollowingSteering.h"
#include "AICharacter.h"
#include "Engine/TargetPoint.h"
#include "Kismet/KismetMathLibrary.h"
#include "params/params.h"
#include "debug/debugdraw.h"

PathFollowingSteering::PathFollowingSteering()
{
}

PathFollowingSteering::~PathFollowingSteering()
{
}

FVector PathFollowingSteering::GetSteering(AAICharacter* _pChar, float DeltaTime)
{
	Params params = _pChar->GetParams();
	
	TArray<FVector> pointPositions;

	for (int i = 0; i < _pChar->m_pathTargets.Num(); ++i)
	{
		pointPositions.Push(_pChar->m_pathTargets[i]->GetActorLocation());
	}

	const float PredictDist = 125.f;
	FVector CurrentPredictedPos = _pChar->GetActorLocation() + _pChar->m_currentVelocity.GetSafeNormal() * PredictDist;
	//DrawDebugSphere(_pChar->GetWorld(), CurrentPredictedPos, 5, 5, FColor::Black);
	int closestSegmentIndex = 0;
	int endSegmentIndex = 0;
	float currentClosestDist = 0;
	
	for (int i = 0; i < pointPositions.Num(); ++i)
	{
		int nextIndex = i + 1;
		if(nextIndex >= pointPositions.Num())
		{
			nextIndex = 0;
		}
		
		FVector closestPoint = UKismetMathLibrary::FindClosestPointOnSegment(CurrentPredictedPos, pointPositions[i], pointPositions[nextIndex]);
		FVector closest = CurrentPredictedPos - closestPoint;
		
		if(closest.Length() < currentClosestDist || currentClosestDist == 0)
		{
			currentClosestDist = closest.Length();
			closestSegmentIndex = i;
			endSegmentIndex = nextIndex;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,TEXT("StartSegment Is: " + FString::FromInt(closestSegmentIndex)));
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,TEXT("EndSegment is: " + FString::FromInt(endSegmentIndex)));

	FVector closestPointToPath = UKismetMathLibrary::FindClosestPointOnSegment(CurrentPredictedPos, pointPositions[closestSegmentIndex], pointPositions[endSegmentIndex]);
	
	FVector PredictionOnPath = closestPointToPath + _pChar->m_currentVelocity.GetSafeNormal() * PredictDist;

	FVector point1 = pointPositions[closestSegmentIndex];
	FVector point2 = pointPositions[endSegmentIndex];

	float surprasedDistanceFromSegment =IsPointOnSegment(point1,point2, PredictionOnPath);
	
	return FVector::Zero();
}

float PathFollowingSteering::IsPointOnSegment(FVector _pos1, FVector _pos2, FVector _pointToCheck)
{
	if((_pointToCheck.X >= _pos1.X && _pointToCheck.X <= _pos2.X) &&
		_pointToCheck.Y >= _pos1.Y && _pointToCheck.Y <= _pos2.Y)
	{
		return 0;
	}
	else
	{
		
	}
}

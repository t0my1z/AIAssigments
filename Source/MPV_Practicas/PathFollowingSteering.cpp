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
	//Get the positions of our pathTargets and store them
	TArray<FVector> pointPositions;
	for (int i = 0; i < _pChar->m_pathTargets.Num(); ++i)
	{
		pointPositions.Push(_pChar->m_pathTargets[i]->GetActorLocation());
	}
	
	//Calculate the nearest point in the path, and its next point
	FVector NearestPoint = FVector::Zero();
	FVector NextPoint = FVector::Zero();
	int endSegmentIndex = 0;
	float currentClosestDist = 0;
	for (int i = 0; i < pointPositions.Num(); ++i)
	{
		int nextIndex = i + 1;
		if(nextIndex >= pointPositions.Num())
		{
			nextIndex = 0;
		}
		
		FVector closestPoint = UKismetMathLibrary::FindClosestPointOnSegment(_pChar->GetActorLocation(), pointPositions[i], pointPositions[nextIndex]);

		float distanceToPoint = FVector::Dist(closestPoint, _pChar->GetActorLocation());
		if(distanceToPoint < currentClosestDist || currentClosestDist == 0)
		{
			NearestPoint = closestPoint;
			NextPoint = pointPositions[nextIndex];
			currentClosestDist = distanceToPoint;
			endSegmentIndex = nextIndex;
		}
	}
	SetCircle(_pChar, TEXT("predictedPathPos"), NearestPoint, 15, FColor::Cyan);

	//Using the calculated nearest point, predict the position we will be in using the next point
	float lookAheadDist = _pChar->GetParams().look_ahead;
	FVector dirFromPointToNext = NextPoint - NearestPoint;
	dirFromPointToNext.Normalize();

	FVector PredictedPos = dirFromPointToNext * lookAheadDist;
	PredictedPos += NearestPoint;

	//After calculating the predicted position, we need to check if it's overshooting, and in case it is
	//update our next point and recalculate the predicted position
	float distFromNearestPointToPredictedPos = FVector::Dist(NearestPoint, PredictedPos);
	float distToNextPointFromNearest = FVector::Dist(NearestPoint, NextPoint);
	if(distFromNearestPointToPredictedPos >= distToNextPointFromNearest)
	{
		int nextIndex = endSegmentIndex + 1;
		if(nextIndex >= pointPositions.Num())
		{
			nextIndex = 0;
		}

		PredictedPos = FVector::Zero();
		FVector dirFromPointToNewNext = pointPositions[nextIndex] - NextPoint;
		dirFromPointToNewNext.Normalize();
		PredictedPos = dirFromPointToNewNext * (distFromNearestPointToPredictedPos - distToNextPointFromNearest);
		PredictedPos += NextPoint;
	}

	SetCircle(_pChar, TEXT("endPredictedPathPos"), PredictedPos, 15, FColor::Black);

	//we return the raw predictedPosition because we then use our Seek behaviour to correctly calculate acceleration
	return PredictedPos;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SteeringBase.h"

/**
 * 
 */
class MPV_PRACTICAS_API PathFollowingSteering : public SteeringBase
{
public:
	PathFollowingSteering();
	virtual ~PathFollowingSteering();
	
	virtual FVector GetSteering(AAICharacter* _pChar, float DeltaTime) override;

	float IsPointOnSegment(FVector _pos1, FVector _pos2, FVector _pointToCheck);
};

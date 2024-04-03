// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AAICharacter;

/**
 * 
 */
class MPV_PRACTICAS_API SteeringBase
{
public:
	SteeringBase();
	virtual ~SteeringBase();

	virtual FVector GetSteering(AAICharacter* _pChar, float DeltaTime) = 0;
};

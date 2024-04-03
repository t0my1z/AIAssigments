// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SteeringBase.h"

/**
 * 
 */
class MPV_PRACTICAS_API SeekSteering : public SteeringBase
{
public:
	SeekSteering();
	virtual ~SeekSteering() override;

	virtual FVector GetSteering(AAICharacter* _pChar, float DeltaTime) override;
};

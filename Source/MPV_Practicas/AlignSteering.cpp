// Fill out your copyright notice in the Description page of Project Settings.


#include "AlignSteering.h"
#include "AICharacter.h"
#include "params/params.h"

AlignSteering::AlignSteering()
{
}

AlignSteering::~AlignSteering()
{
}

FVector AlignSteering::GetSteering(AAICharacter* _pChar, float DeltaTime)
{
	Params params = _pChar->GetParams();

	float angle = params.targetRotation - _pChar->GetActorAngle();
	if (angle > 180.f)
	{
		angle -= 360;
	}
	else if (angle < -180.f)
	{
		angle += 360;
	}

	float aDirection = FMath::Sign(angle);

	float aVelocity = FMath::Min(FMath::Abs(angle), params.max_angular_acceleration);

	FVector angularAcceleration(0.0f, aDirection * aVelocity, 0.0f);

	return angularAcceleration;
}

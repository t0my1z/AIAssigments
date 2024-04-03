// Fill out your copyright notice in the Description page of Project Settings.


#include "ArriveSteering.h"
#include "AICharacter.h"
#include "params/params.h"

ArriveSteering::ArriveSteering()
{
}

ArriveSteering::~ArriveSteering()
{
}

FVector ArriveSteering::GetSteering(AAICharacter* _pChar, float DeltaTime)
{
	Params params = _pChar->GetParams();

	FVector dir = params.targetPosition - _pChar->GetActorLocation();

	float distance = dir.Size();

	dir.Normalize();

	FVector newVelocity;

	if (distance > params.deceleration_radius)
	{
		newVelocity = dir * params.max_velocity;
	}
	else
	{
		newVelocity = (dir * params.max_velocity) * (distance / params.deceleration_radius);
	}

	FVector newAcceleration = newVelocity - _pChar->m_currentVelocity;

	newAcceleration = newAcceleration.GetSafeNormal() * params.max_acceleration;
	newAcceleration.GetClampedToMaxSize(params.max_acceleration);

	return newAcceleration * DeltaTime;
}

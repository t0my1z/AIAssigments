// Fill out your copyright notice in the Description page of Project Settings.


#include "SeekSteering.h"
#include "AICharacter.h"
#include "params/params.h"

SeekSteering::SeekSteering()
{
}

SeekSteering::~SeekSteering()
{
}

FVector SeekSteering::GetSteering(AAICharacter* _pChar, float DeltaTime)
{
	Params params = _pChar->GetParams();

	FVector dir = params.targetPosition - _pChar->GetActorLocation();
	FVector newVelocity = dir.GetSafeNormal() * params.max_velocity;
	FVector newAcceleration = newVelocity - _pChar->m_currentVelocity;

	newAcceleration = newAcceleration.GetSafeNormal() * params.max_acceleration;
	newAcceleration.GetClampedToMaxSize(params.max_acceleration);

	return newAcceleration * DeltaTime;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "params/params.h"
#include "debug/debugdraw.h"
#include "SeekSteering.h"
#include "ArriveSteering.h"
#include "AlignSteering.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	ReadParams("params.xml", m_params);
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	current_angle = GetActorAngle();

	ApplySteeringBehaviors(DeltaTime);

	DrawDebug();
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::OnClickedLeft(const FVector& mousePosition)
{
	SetActorLocation(mousePosition);
}

void AAICharacter::OnClickedRight(const FVector& mousePosition)
{
	m_params.targetPosition = mousePosition;

	FVector dir = (mousePosition - GetActorLocation()).GetSafeNormal();
	float angle = FMath::RadiansToDegrees(atan2(dir.Z, dir.X));
	m_params.targetRotation = angle;
}

void AAICharacter::ApplySteeringBehaviors(float DeltaTime)
{
	m_currentSteering = NULL;
	m_currentRotationSteering = NULL;
	switch (m_currentBehavior)
	{
	case SteeringBehavior::Seek:

		m_currentSteering = new SeekSteering();
		break;
	case SteeringBehavior::Arrive:

		m_currentSteering = new ArriveSteering();
		break;
	case SteeringBehavior::Align:

		m_currentRotationSteering = new AlignSteering();
		break;
	case SteeringBehavior::ArriveAndAlign:

		m_currentSteering = new ArriveSteering();
		m_currentRotationSteering = new AlignSteering();
		break;
	case SteeringBehavior::Pursue:
		break;
	default:
		break;
	}

	if (m_currentSteering)
	{
		m_currentVelocity += m_currentSteering->GetSteering(this, DeltaTime);

		m_currentVelocity.GetClampedToMaxSize(m_params.max_velocity);

		SetActorLocation(GetActorLocation() + m_currentVelocity * DeltaTime);
	}

	if (m_currentRotationSteering)
	{
		FVector angularAcceleration = m_currentRotationSteering->GetSteering(this, DeltaTime);
		float currentRotationAngle = GetActorAngle();
		currentRotationAngle += angularAcceleration.Y * DeltaTime;

		currentRotationAngle = convertTo360(currentRotationAngle);

		FRotator newRotation = FRotator(currentRotationAngle, 0, 0);
		SetActorRotation(newRotation);
	}
}

void AAICharacter::DrawDebug()
{
	TArray<FVector> Points =
	{
		FVector(0.f, 0.f, 0.f),
		FVector(100.f, 0.f, 0.f),
		FVector(100.f, 0.f, 100.f),
		FVector(100.f, 0.f, 100.f),
		FVector(0.f, 0.f, 100.f)
	};

	SetPath(this, TEXT("follow_path"), TEXT("path"), Points, 5.0f, PathMaterial);

	SetCircle(this, TEXT("targetPosition"), m_params.targetPosition, m_params.deceleration_radius);
	FVector dir(cos(FMath::DegreesToRadians(m_params.targetRotation)), 0.0f, sin(FMath::DegreesToRadians(m_params.targetRotation)));
	SetArrow(this, TEXT("targetRotation"), dir, 80.0f);

	TArray<TArray<FVector>> Polygons = {
		{ FVector(0.f, 0.f, 0.f), FVector(100.f, 0.f, 0.f), FVector(100.f, 0.f, 100.0f), FVector(0.f, 0.f, 100.0f) },
		{ FVector(100.f, 0.f, 0.f), FVector(200.f, 0.f, 0.f), FVector(200.f, 0.f, 100.0f) }
	};
	SetPolygons(this, TEXT("navmesh"), TEXT("mesh"), Polygons, NavmeshMaterial);
}
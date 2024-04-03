// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "params/params.h"
#include "GameFramework/PlayerController.h"
#include "util.h"


#include "AICharacter.generated.h"

class SteeringBase;

UENUM(BlueprintType)
enum class SteeringBehavior : uint8
{
	Seek UMETA(DisplayName = "Seek"),
	Arrive UMETA(DisplayName = "Arrive"),
	Align UMETA(DisplayName = "Align"),
	ArriveAndAlign UMETA(DisplayName = "ArriveAndAlign"),
	Pursue UMETA(DisplayName = "Pursue")
};

UCLASS()
class MPV_PRACTICAS_API AAICharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAICharacter();

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIChar)
	uint32 bDoMovement : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIChar)
	float current_angle;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* PathMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* NavmeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIChar)
	SteeringBehavior m_currentBehavior;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AIChar)
	FVector m_currentVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AIChar)
	FVector m_currentAngularVelocity;



	SteeringBase* m_currentSteering;
	SteeringBase* m_currentRotationSteering;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	Params m_params;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "AIFunctions")
	void OnClickedLeft(const FVector& mousePosition);
	UFUNCTION(BlueprintCallable, Category = "AIFunctions")
	void OnClickedRight(const FVector& mousePosition);
	
	const Params& GetParams() const { return m_params; }

	float GetActorAngle() const
	{
		FQuat currQuat = GetActorQuat();
		FVector axis;
		float axisAngle;
		currQuat.ToAxisAndAngle(axis, axisAngle);
		axisAngle = FMath::RadiansToDegrees(axisAngle);
		if (axis.Y > 0.0f)
			axisAngle = -axisAngle;

		axisAngle = convertTo360(axisAngle);
		return axisAngle;
	}
	void SetActorAngle(float angle) { FRotator newRot(angle, 0.0f, 0.0f); SetActorRotation(newRot); }

	void ApplySteeringBehaviors(float DeltaTime);

	void DrawDebug();

};
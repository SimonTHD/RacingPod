#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnakinPod.generated.h"

UCLASS()
class SWRACINGPOD_API AAnakinPod : public ACharacter
{
	GENERATED_BODY()

public:
	AAnakinPod();

	UPROPERTY(EditAnyWhere, Category = "Flight")
		float Acceleration{ 30.f };
	UPROPERTY(EditAnyWhere, Category = "Flight")
		float MaxSpeed{ 1000.f };
	UPROPERTY(EditAnyWhere, Category = "Flight")
		float MinSpeed{ 0.f };

	UPROPERTY(EditAnyWhere, Category = "Flight")
		float RollRateMultiplier{ 200.f };
	UPROPERTY(EditAnyWhere, Category = "Flight")
		float PitchRateMultiplier{ 200.f };

	/*UPROPERTY(VisibleAnyWhere, Category = "Flight")
		float CurrentForwardSpeed{ 0.f };*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
		float CurrentForwardSpeed = 0.f;

	float CurrentYawSpeed;
	float CurrentPitchSpeed;
	float CurrentRollSpeed;

	bool bIntentionalPitch{ false };
	bool bIntentionalRoll{ false };

protected:

	void ProcessKeyPitch(float Rate);
	void ProcessKeyRoll(float Rate);

	void ProcessMouseYInput(float Value);
	void ProcessMouseXInput(float Value);

	//Calculating actor rotation
	void ProcessRoll(float Value);
	void ProcessPitch(float Value);

	//Acceleration
	bool bAccelerating{ false };

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void StartAccelerating();
	void StopAccelerating();
};

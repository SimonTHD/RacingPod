// Fill out your copyright notice in the Description page of Project Settings.


#include "AnakinPod.h"

// Sets default values
AAnakinPod::AAnakinPod()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAnakinPod::ProcessKeyPitch(float Rate)
{
	if (FMath::Abs(Rate) > .2f)
		ProcessPitch(Rate * 2.f);
}

void AAnakinPod::ProcessKeyRoll(float Rate)
{
	if (FMath::Abs(Rate) > .2f)
		ProcessRoll(Rate * 2.f);
}

void AAnakinPod::ProcessMouseYInput(float Value)
{
	ProcessPitch(Value);
}

void AAnakinPod::ProcessMouseXInput(float Value)
{
	ProcessRoll(Value);
}

void AAnakinPod::ProcessRoll(float Value)
{
	bIntentionalRoll = FMath::Abs(Value) > 0.f;

	if (bIntentionalPitch && !bIntentionalPitch) return;

	const float TargetRollSpeed = bIntentionalRoll ? (Value * RollRateMultiplier) : (GetActorRotation().Roll * -2.f);

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AAnakinPod::ProcessPitch(float Value)
{
	bIntentionalPitch = FMath::Abs(Value) > 0.f;

	const float TargetPitchSpeed = Value * PitchRateMultiplier;

	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

// Called when the game starts or when spawned
void AAnakinPod::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnakinPod::Tick(float DeltaTime)
{
	// Calculate Thrust
	const float CurrentAcc = -GetActorRotation().Pitch * DeltaTime * Acceleration;
	const float NewForwardSpeed = CurrentForwardSpeed + CurrentAcc;
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);

	// Move the actor locally
	FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);
	AddActorLocalOffset(LocalMove, true);

	// Calculate rotation deltas
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;

	// Add local rotation
	AddActorLocalRotation(DeltaRotation);

	// Update actor's location to stay within specified height range
	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z = FMath::Clamp(ActorLocation.Z, 50.f, 600.f);  // Limit height between 100 and 600
	SetActorLocation(ActorLocation);

	// Debug messages
	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, FString::Printf(TEXT("ForwardSpeed: %f"), CurrentForwardSpeed));
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Yellow, FString::Printf(TEXT("Actor Location: %s"), *ActorLocation.ToString()));
	GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Blue, FString::Printf(TEXT("Roll Speed: %f"), CurrentRollSpeed));
	GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Red, FString::Printf(TEXT("Pitch Speed: %f"), CurrentPitchSpeed));

	Super::Tick(DeltaTime);
}

void AAnakinPod::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//Deflect along surface
	const FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
	
	//Slow Down
	CurrentForwardSpeed = FMath::FInterpTo(CurrentForwardSpeed, MinSpeed, GetWorld()->GetDeltaSeconds(), 5.f);
}

// Called to bind functionality to input
void AAnakinPod::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookRight", this, &AAnakinPod::ProcessMouseXInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AAnakinPod::ProcessMouseYInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAnakinPod::ProcessKeyRoll);
	PlayerInputComponent->BindAxis("MoveForward", this, &AAnakinPod::ProcessKeyPitch);
}


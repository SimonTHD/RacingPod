#include "AnakinPod.h"

AAnakinPod::AAnakinPod()
{
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

void AAnakinPod::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAnakinPod::Tick(float DeltaTime)
{
	const float CurrentAcc = -GetActorRotation().Pitch * DeltaTime * Acceleration;

	const float NewForwardSpeed = bAccelerating ? (CurrentForwardSpeed + CurrentAcc) : (CurrentForwardSpeed - CurrentAcc);

	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);

	FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);
	AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;

	AddActorLocalRotation(DeltaRotation);

	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z = FMath::Clamp(ActorLocation.Z, -10000.f, 2000.f);  
	SetActorLocation(ActorLocation);


	/*GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, FString::Printf(TEXT("ForwardSpeed: %f"), CurrentForwardSpeed));
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Yellow, FString::Printf(TEXT("Actor Location: %s"), *ActorLocation.ToString()));
	GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Blue, FString::Printf(TEXT("Roll Speed: %f"), CurrentRollSpeed));
	GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Red, FString::Printf(TEXT("Pitch Speed: %f"), CurrentPitchSpeed));
	GEngine->AddOnScreenDebugMessage(4, 0.f, FColor::Purple, FString::Printf(TEXT("Current Acc: %f"), CurrentAcc));*/

	Super::Tick(DeltaTime);
}

void AAnakinPod::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	const FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));

	CurrentForwardSpeed = FMath::FInterpTo(CurrentForwardSpeed, MinSpeed, GetWorld()->GetDeltaSeconds(), 5.f);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Collision détectée!"));

	Destroy();
}

void AAnakinPod::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookRight", this, &AAnakinPod::ProcessMouseXInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AAnakinPod::ProcessMouseYInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAnakinPod::ProcessKeyRoll);
	PlayerInputComponent->BindAxis("MoveForward", this, &AAnakinPod::ProcessKeyPitch);

	PlayerInputComponent->BindAction("Accelerate", IE_Pressed, this, &AAnakinPod::StartAccelerating);
	PlayerInputComponent->BindAction("Accelerate", IE_Released, this, &AAnakinPod::StopAccelerating);
}

void AAnakinPod::StartAccelerating()
{
	bAccelerating = true;
	MinSpeed = 9000.f;
	MaxSpeed = 14000.f;
	CurrentForwardSpeed = 500.f;
}

void AAnakinPod::StopAccelerating()
{
	bAccelerating = false;
	MinSpeed = 1500.f;
	MaxSpeed = 3000.f;
}


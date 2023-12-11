// Copyright Epic Games, Inc. All Rights Reserved.

#include "SWRacingPodWheelFront.h"
#include "UObject/ConstructorHelpers.h"

USWRacingPodWheelFront::USWRacingPodWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}
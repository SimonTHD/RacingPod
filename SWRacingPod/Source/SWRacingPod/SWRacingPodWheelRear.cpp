// Copyright Epic Games, Inc. All Rights Reserved.

#include "SWRacingPodWheelRear.h"
#include "UObject/ConstructorHelpers.h"

USWRacingPodWheelRear::USWRacingPodWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}
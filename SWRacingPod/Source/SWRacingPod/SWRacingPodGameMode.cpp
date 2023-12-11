// Copyright Epic Games, Inc. All Rights Reserved.

#include "SWRacingPodGameMode.h"
#include "SWRacingPodPlayerController.h"

ASWRacingPodGameMode::ASWRacingPodGameMode()
{
	PlayerControllerClass = ASWRacingPodPlayerController::StaticClass();
}

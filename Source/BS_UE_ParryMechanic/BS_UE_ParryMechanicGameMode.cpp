// Copyright Epic Games, Inc. All Rights Reserved.

#include "BS_UE_ParryMechanicGameMode.h"
#include "BS_UE_ParryMechanicCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABS_UE_ParryMechanicGameMode::ABS_UE_ParryMechanicGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

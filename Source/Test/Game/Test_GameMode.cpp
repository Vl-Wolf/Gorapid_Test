// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test_GameMode.h"
#include "Test/Character/Test_Character.h"
#include "UObject/ConstructorHelpers.h"

ATest_GameMode::ATest_GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/Character/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

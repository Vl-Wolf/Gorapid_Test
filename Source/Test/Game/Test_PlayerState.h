// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Test/FuncLibrary/Types.h"
#include "Test_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API ATest_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ATest_PlayerState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<FWeaponSlot> WeaponSlot;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Test/FuncLibrary/Types.h"
#include "Test_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UTest_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponSetting")
	UDataTable* WeaponInfoTable = nullptr;

	UFUNCTION(BlueprintCallable)
	bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo &OutInfo);
	
};

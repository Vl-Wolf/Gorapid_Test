﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "TT_GameInstance.h"

bool UTT_GameInstance::GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo)
{
	bool bIsFind = false;

	if (WeaponInfoTable)
	{
		FWeaponInfo* WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponInfo>(NameWeapon, "", false);
		if (WeaponInfoRow)
		{
			bIsFind = true;
			OutInfo = *WeaponInfoRow;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTT_GameInstance::GetWeaponInfoByName - WeaponTable -NULL"))
	}
	
	return bIsFind;
}


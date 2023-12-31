﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types.generated.h"

/**
 * 
 */
UCLASS()
class TT_API UTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Aim_State UMETA(DisplayName = "Aim State"),
	Walk_State UMETA(DisplayName = "Walk State"),
	Sprint_State UMETA(DisplayName = "Sprint State")
};

UENUM(BlueprintType)
enum class EBehaviorState : uint8
{
	Wait_State UMETA(DisplayName = "Wait State"),
	Aggressive_State UMETA(DisplayName = "Aggressive State"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	RifleType UMETA(DisplayName = "Rifle"),
};

USTRUCT(BlueprintType)
struct FCharacterSpeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float AimSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float SprintSpeed = 500.0f;
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	TSubclassOf<class AProjectileDefault> Projectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	UStaticMesh* ProjectileStaticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	FTransform ProjectileStaticMeshOffset = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	UNiagaraSystem* ProjectileTrailFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	FTransform ProjectileTrailFXOffset = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	float ProjectileDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	float ProjectileLifeTime = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	float ProjectileInitSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileSetting")
	float ProjectileMaxSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileFX")
	TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> HitDecals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileFX")
	USoundBase* HitSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileFX")
	TMap<TEnumAsByte<EPhysicalSurface>, UNiagaraSystem*> HitFXs;
};

USTRUCT(BlueprintType)
struct FAnimationWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation Character")
	UAnimMontage* AnimCharacterFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation Character")
	UAnimMontage* AnimCharacterFireAim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation Character")
	UAnimMontage* AnimCharacterReload = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation Character")
	UAnimMontage* AnimCharacterReloadAnim = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation Character")
	UAnimMontage* AnimWeaponFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation Character")
	UAnimMontage* AnimWeaponReload = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation Character")
	UAnimMontage* AnimWeaponReloadAnim = nullptr;
	
};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Class")
	TSubclassOf<class AWeaponDefault> WeaponClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	float RateOfFire = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	float ReloadTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	int32 MaxRound = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* SoundWeaponFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* SoundWeaponReload = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FX")
	UNiagaraSystem* WeaponFireEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	FProjectileInfo ProjectileInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
	float WeaponDamage = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tarce")
	float DistanceTrace = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitDecal")
	UDecalComponent* DecalOnHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	FAnimationWeaponInfo AnimationWeaponInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	float SwitchTimeWeapon = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	UTexture2D* WeaponIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	EWeaponType WeaponType = EWeaponType::RifleType;
	
};

USTRUCT(BlueprintType)
struct FAdditionalWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponStats")
	int32 Round = 0;
};

USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponSlot")
	FName NameItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponSlot")
	FAdditionalWeaponInfo AdditionalWeaponInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponSlot")
	EWeaponType WeaponType = EWeaponType::RifleType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponSlot")
	int32 Count = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponSlot")
	int32 MaxCount = 100;
	
};


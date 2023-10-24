// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TT/FuncLibrary/Types.h"
#include "WeaponDefault.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloadStart, UAnimMontage*, AnimReloadStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponReloadEnd, bool, bIsSucces, int32, AmmoSafe);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFire, UAnimMontage*, AnimFireCharacter);

UCLASS()
class TT_API AWeaponDefault : public AActor
{
	GENERATED_BODY()

public:
	
	AWeaponDefault();

	FOnWeaponReloadStart OnWeaponReloadStart;
	FOnWeaponReloadEnd OnWeaponReloadEnd;
	FOnWeaponFire OnWeaponFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Components")
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Components")
	USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Components")
	UStaticMeshComponent* StaticMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Components")
	UArrowComponent* ShootLocation = nullptr;

	UPROPERTY(VisibleAnywhere)
	FWeaponInfo WeaponInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponInfo")
	FAdditionalWeaponInfo AdditionalWeaponInfo;

	float FireTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload")
	float ReloadTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fire")
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fire")
	bool WeaponFiring = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Reload")
	bool WeaponReloading = false;

	bool WeaponAiming = false;
	bool BlockFire = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ShootEndLocation = FVector(0);

protected:
	
	virtual void BeginPlay() override;

	void FireTick(float DeltaTime);
	void ReloadTick(float DeltaTime);

	void WeaponInit();

	UFUNCTION()
	void Fire();

public:
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponStateFire(bool bIsFire);

	bool CheckWeaponCanFire();

	FProjectileInfo GetProjectile();
	
	UFUNCTION(BlueprintCallable)
	int32 GetWeaponRound();

	UFUNCTION()
	void InitReload();
	void FinishReload();
	void CancelReload();

	bool CheckCanWeaponReload();

	int8 GetAvailableAmmoForReload();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool ShowDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	float SizeVectorToChangeShootDirectionLogic = 100.0f;
	
	UFUNCTION()
	void AnimWeaponStart(UAnimMontage* Anim);

	UFUNCTION()
	void FXWeaponFire(UNiagaraSystem* FXFire, USoundBase* SoundFire);

	UFUNCTION()
	void SpawnTraceHitDecal(UMaterialInterface* DecalMaterial, FHitResult HitResult);
	UFUNCTION()
	void SpawnTraceHitFX(UNiagaraSystem* FX, FHitResult HitResult);
	UFUNCTION()
	void SpawnTraceHitSound(USoundBase* HitSound, FHitResult HitResult);

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};

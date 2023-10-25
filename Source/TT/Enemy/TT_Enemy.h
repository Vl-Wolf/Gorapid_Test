// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TT/Character/TT_HealthComponent.h"
#include "TT/Character/TT_InventoryComponent.h"
#include "TT_Enemy.generated.h"

UCLASS()
class TT_API ATT_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void InitWeapon(FName WeaponName, FAdditionalWeaponInfo AdditionalWeaponInfo, int32 NewCurrentIndexWeapon);
	
	ATT_Enemy();

	UFUNCTION()
	UTT_InventoryComponent* GetInventoryComponent() const
	{
		return InventoryComponent;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<FWeaponSlot> WeaponSlot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UTT_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UTT_InventoryComponent* InventoryComponent;

	UPROPERTY()
	AWeaponDefault* CurrentWeapon = nullptr;

	UPROPERTY()
	int32 CurrentIndexWeapon = 0;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetCharacterDeathMontage();

	UFUNCTION(BlueprintCallable)
	void DeadEvent();

	UFUNCTION(BlueprintNativeEvent)
	void CharacterDead();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Death")
	TArray<UAnimMontage*> DeathAnim;

	UFUNCTION(BlueprintCallable)
	void EnableRagdoll();

	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd(bool bIsSuccess, int32 AmmoSafe);
	UFUNCTION()
	void WeaponFire(UAnimMontage* Anim);

	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFire_BP(UAnimMontage* Anim);

	UFUNCTION()
	void AttackEvent(bool bIsFiring);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponDefault* GetCurrentWeapon();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

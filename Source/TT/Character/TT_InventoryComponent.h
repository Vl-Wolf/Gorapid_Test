// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TT/FuncLibrary/Types.h"
#include "TT_InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSwitchWeapon, FName, WeaponIdName, FAdditionalWeaponInfo, WeaponAdditionalInfo, int32, NewCurrentIndexWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange, EWeaponType, TypeAmmo, int32, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponAdditionalInfoChange, int32, IndexSlot, FAdditionalWeaponInfo, AdditionalInfo);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TT_API UTT_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UTT_InventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSwitchWeapon OnSwitchWeapon;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnAmmoChange OnAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponAdditionalInfoChange OnWeaponAdditionalInfoChange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TArray<FWeaponSlot> WeaponSlots;

	int32 MaxSlotsWeapon = 0;

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool SwitchWeaponByIndex(int32 IndexWeaponToChange, int32 PreviousIndex, FAdditionalWeaponInfo PreviousWeaponInfo);

	void SetAdditionalInfoWeapon(int32 IndexWeapon, FAdditionalWeaponInfo NewInfo);

	FAdditionalWeaponInfo GetAdditionalInfoWeapon(int32 IndexWeapon);
	int32 GetWeaponIndexSlotByName(FName IdWeaponName);
	FName GetWeaponNameBySlotIndex(int32 IndexSlot);
	bool GetWeaponTypeByIndexSlot(int32 IndexSlot, EWeaponType& WeaponType);
	bool GetWeaponTypeByNameWeapon(FName IdWeaponName, EWeaponType& WeaponType);

	UFUNCTION(BlueprintCallable)
	void AmmoSlotChangeValue(EWeaponType TypeWeapon, int32 CountChangeAmmo);
	bool CheckAmmoForWeapon(EWeaponType TypeWeapon, int8& AvailableAmmForWeapon);
	
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool CheckCanTakeAmmo(EWeaponType AmmoType);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool CheckCanTakeWeapon(int32& FreeSlot);

	UFUNCTION(BlueprintCallable, Category = "Interface")
	void TryGetWeaponToInventory(AActor* PickUpActor, FWeaponSlot NewWeapon);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FWeaponSlot> GetWeaponSlots();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitInventory(const TArray<FWeaponSlot>& NewWeaponSlotsInfo);

	UFUNCTION()
	void SwitchWeaponEvent(FName WeaponName, FAdditionalWeaponInfo AdditionalInfo, int32 IndexSlot);
	UFUNCTION()
	void AmmoChangeEvent(EWeaponType TypeWeapon, int32 Count);
	UFUNCTION()
	void WeaponAdditionalInfoChangeEvent(int32 IndexWeapon, FAdditionalWeaponInfo AdditionalWeaponInfo);
};

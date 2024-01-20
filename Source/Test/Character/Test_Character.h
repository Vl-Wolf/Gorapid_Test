// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Test_HealthComponent.h"
#include "Test_InventoryComponent.h"
#include "GameFramework/Character.h"
#include "Test/FuncLibrary/Types.h"
#include "Test_Character.generated.h"

UCLASS(config=Game)
class ATest_Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	virtual void Tick(float DeltaSeconds) override;
public:
	ATest_Character();
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	UPROPERTY()
	EMovementState MovementState = EMovementState::Walk_State;
	UPROPERTY()
	AWeaponDefault* CurrentWeapon = nullptr;

	UPROPERTY()
	int32 CurrentIndexWeapon = 0;

	UFUNCTION()
	void CharacterDead();
	UFUNCTION()
	void EnableRagdoll();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UTest_HealthComponent* Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UTest_InventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	FCharacterSpeed CharacterSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Death")
	TArray<UAnimMontage*> DeadAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Death")
	UParticleSystem* DeadFX;

public:

	FTimerHandle TimerHandle_RagdollTimer;

	UFUNCTION()
	UTest_InventoryComponent* GetInventoryComponent() const
	{
		return InventoryComponent;
	}

	void CharacterUpdate();
	void ChangeMovementState();

	void AttackEvent(bool bIsFiring);
	void AimEvent(bool bIsAiming);
	void SprintEvent();

	UFUNCTION()
	void InitWeapon(FName WeaponName, FAdditionalWeaponInfo AdditionalWeaponInfo, int32 NewCurrentIndexWeapon);

	void TryReloadWeapon();

	UFUNCTION()
	void WeaponFire(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd(bool bIsSuccess, int32 AmmoSafe);

	UFUNCTION()
	void TrySwitchWeapon(int32 ToIndex);

	void TrySwitchWeapon();

	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFire_BP(UAnimMontage* Anim);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponDefault* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EMovementState GetMovementState();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCurrentWeaponIndex();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsAlive();

	UFUNCTION(BlueprintNativeEvent)
	void CharacterDead_BP();

	UFUNCTION()
	void SetActorRotationByYaw(float Yaw);
	
	UFUNCTION()
	void SetMovementState(EMovementState NewState);
	
	UFUNCTION()
	void PlayAnim(UAnimMontage* Anim);

private:
	
	virtual void BeginPlay() override;

	void RotateWeaponTick(AWeaponDefault* Weapon);
	
};


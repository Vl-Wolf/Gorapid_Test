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
	// Sets default values for this character's properties
	ATT_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UTT_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	UTT_InventoryComponent* InventoryComponent;

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

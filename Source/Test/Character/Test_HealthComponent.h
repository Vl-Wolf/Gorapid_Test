﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Test_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEST_API UTest_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTest_HealthComponent();

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Health")
	FOnHealthChange OnHealthChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Helath")
	FOnDead OnDead;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	float Health = 100.0f;
	bool bIsAlive = true;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float CoefDamage = 1.0f;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetCurrentHealth();
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float NewHealth);
	UFUNCTION(BlueprintCallable, Category="Health")
	bool GetIsAlive();

	UFUNCTION(BlueprintCallable, Category="Health")
	void ChangeHealthValue(float ChangeValue);
	UFUNCTION(BlueprintCallable, Category="Health")
	void HealthChange(float NewHealth, float Value);
	UFUNCTION()
	void DeadEvent();
		
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_HealthComponent.h"


UTest_HealthComponent::UTest_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTest_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTest_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UTest_HealthComponent::GetCurrentHealth()
{
	return Health;
}

void UTest_HealthComponent::SetCurrentHealth(float NewHealth)
{
	Health = NewHealth;
}

bool UTest_HealthComponent::GetIsAlive()
{
	return bIsAlive;
}

void UTest_HealthComponent::ChangeHealthValue(float ChangeValue)
{
	if (bIsAlive)
	{
		ChangeValue *= CoefDamage;

		Health += ChangeValue;

		HealthChange(Health, ChangeValue);

		if (Health > 100.0f)
		{
			Health = 100.0f;
		}
		else
		{
			if (Health <= 0.0f)
			{
				bIsAlive = false;
				DeadEvent();
			}
		}
	}
}

void UTest_HealthComponent::HealthChange(float NewHealth, float Value)
{
	OnHealthChange.Broadcast(NewHealth, Value);
}

void UTest_HealthComponent::DeadEvent()
{
	OnDead.Broadcast();
}

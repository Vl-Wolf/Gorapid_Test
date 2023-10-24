// Fill out your copyright notice in the Description page of Project Settings.


#include "TT_HealthComponent.h"


UTT_HealthComponent::UTT_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTT_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTT_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UTT_HealthComponent::GetCurrentHealth()
{
	return Health;
}

void UTT_HealthComponent::SetCurrentHealth(float NewHealth)
{
	Health = NewHealth;
}

bool UTT_HealthComponent::GetIsAlive()
{
	return bIsAlive;
}

void UTT_HealthComponent::ChangeHealthValue(float ChangeValue)
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
			if (Health < 0.0f)
			{
				bIsAlive = false;
				DeadEvent();
			}
		}
	}
}

void UTT_HealthComponent::HealthChange(float NewHealth, float Value)
{
	OnHealthChange.Broadcast(NewHealth, Value);
}

void UTT_HealthComponent::DeadEvent()
{
	OnDead.Broadcast();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "TT_Enemy.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ATT_Enemy::ATT_Enemy()
{
	
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UTT_HealthComponent>(TEXT("HealthComponent"));
	InventoryComponent = CreateDefaultSubobject<UTT_InventoryComponent>(TEXT("InventoryComponent"));

	if (HealthComponent)
	{
		HealthComponent->OnDead.AddDynamic(this, &ATT_Enemy::CharacterDead);
	}
	
}

void ATT_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

float ATT_Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HealthComponent && HealthComponent->GetIsAlive())
	{
		HealthComponent->ChangeHealthValue(-DamageAmount);
	}

	return ActualDamage;
}

UAnimMontage* ATT_Enemy::GetCharacterDeathMontage()
{
	int32 RandomInt = UKismetMathLibrary::RandomInteger(DeathAnim.Num());

	return DeathAnim[RandomInt];
}

void ATT_Enemy::DeadEvent()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetController()->UnPossess();
	SetLifeSpan(10.0f);
}

void ATT_Enemy::CharacterDead_Implementation()
{
}

void ATT_Enemy::EnableRagdoll()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionProfileName("PhysicsActor", true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
}

// Called every frame
void ATT_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


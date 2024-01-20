// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_Enemy.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Test/Game/Test_GameInstance.h"
#include "Test/Weapon/WeaponDefault.h"


ATest_Enemy::ATest_Enemy()
{
	
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UTest_HealthComponent>(TEXT("HealthComponent"));
	InventoryComponent = CreateDefaultSubobject<UTest_InventoryComponent>(TEXT("InventoryComponent"));

	if (HealthComponent)
	{
		HealthComponent->OnDead.AddDynamic(this, &ATest_Enemy::CharacterDead);
	}

	if (InventoryComponent)
	{
		InventoryComponent->OnSwitchWeapon.AddDynamic(this, &ATest_Enemy::InitWeapon);
	}

	WeaponSlot.Add(FWeaponSlot{"Rifle", 45, EWeaponType::RifleType, 45,120});
	
}

void ATest_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

float ATest_Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HealthComponent && HealthComponent->GetIsAlive())
	{
		HealthComponent->ChangeHealthValue(-DamageAmount);
	}

	return ActualDamage;
}

UAnimMontage* ATest_Enemy::GetCharacterDeathMontage()
{
	int32 RandomInt = UKismetMathLibrary::RandomInteger(DeathAnim.Num());

	return DeathAnim[RandomInt];
}

void ATest_Enemy::DeadEvent()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetController()->UnPossess();
	SetLifeSpan(10.0f);
}

void ATest_Enemy::CharacterDead_Implementation()
{
}

void ATest_Enemy::InitWeapon(FName WeaponName, FAdditionalWeaponInfo AdditionalWeaponInfo, int32 NewCurrentIndexWeapon)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	UTest_GameInstance* MyGI = Cast<UTest_GameInstance>(GetGameInstance());
	if (MyGI)
	{
		FWeaponInfo MyWeaponInfo;
		if (MyGI->GetWeaponInfoByName(WeaponName, MyWeaponInfo))
		{
			if (MyWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);
				FActorSpawnParameters SpawnParameters;

				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParameters.Owner = this;
				SpawnParameters.Instigator = GetInstigator();

				AWeaponDefault* MyWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(MyWeaponInfo.WeaponClass,
																	&SpawnLocation, &SpawnRotation, SpawnParameters));

				if (MyWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					MyWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocket"));
					CurrentWeapon = MyWeapon;

					MyWeapon->WeaponName = WeaponName;
					MyWeapon->WeaponInfo = MyWeaponInfo;
					MyWeapon->ReloadTimer = MyWeaponInfo.ReloadTime;
					MyWeapon->AdditionalWeaponInfo = AdditionalWeaponInfo;

					CurrentIndexWeapon = NewCurrentIndexWeapon;
					
					MyWeapon->OnWeaponReloadStart.AddDynamic(this, &ATest_Enemy::WeaponReloadStart);
					MyWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATest_Enemy::WeaponReloadEnd);
					MyWeapon->OnWeaponFire.AddDynamic(this, &ATest_Enemy::WeaponFire);

					if (CurrentWeapon->GetWeaponRound() <= 0 && CurrentWeapon->CheckCanWeaponReload())
					{
						CurrentWeapon->InitReload();
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATTCharacter::InitWeapon - Weapon not found in table -NULL"))
		}
	}
}

void ATest_Enemy::EnableRagdoll()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionProfileName("PhysicsActor", true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
}

void ATest_Enemy::WeaponReloadStart(UAnimMontage* Anim)
{
	WeaponReloadStart_BP(Anim);
}

void ATest_Enemy::WeaponReloadEnd(bool bIsSuccess, int32 AmmoSafe)
{
	if (InventoryComponent&&  CurrentWeapon)
	{
		InventoryComponent->AmmoSlotChangeValue(CurrentWeapon->WeaponInfo.WeaponType, AmmoSafe);
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	}

	WeaponReloadEnd_BP(bIsSuccess);
}

void ATest_Enemy::WeaponFire(UAnimMontage* Anim)
{
	if (InventoryComponent && CurrentWeapon)
	{
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	}
	
	WeaponFire_BP(Anim);
}

void ATest_Enemy::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
}

void ATest_Enemy::WeaponReloadEnd_BP_Implementation(bool bIsSuccess)
{
}

void ATest_Enemy::WeaponFire_BP_Implementation(UAnimMontage* Anim)
{
}

void ATest_Enemy::AttackEvent(bool bIsFiring)
{
	if (HealthComponent && HealthComponent->GetIsAlive())
	{
		AWeaponDefault* MyWeapon = GetCurrentWeapon();
		if (MyWeapon)
		{
			MyWeapon->SetWeaponStateFire(bIsFiring);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATTCharacter::AttackCharEvent - CurrentWeapon -NULL"));
		}
	}
}

AWeaponDefault* ATest_Enemy::GetCurrentWeapon()
{
	return CurrentWeapon;
}

EBehaviorState ATest_Enemy::GetBehaviorState()
{
	return BehaviorState;
}

void ATest_Enemy::SetBehaviorState(EBehaviorState NewBehaviorState)
{
	BehaviorState = NewBehaviorState;
}

// Called every frame
void ATest_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test_Character.h"

#include "Test_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Test/Game/Test_GameInstance.h"
#include "Test/Weapon/WeaponDefault.h"

//////////////////////////////////////////////////////////////////////////
// ATTCharacter

ATest_Character::ATest_Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 100.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;
	// Camera does not rotate relative to arm

	Health = CreateDefaultSubobject<UTest_HealthComponent>(TEXT("HealthComponent"));
	InventoryComponent = CreateDefaultSubobject<UTest_InventoryComponent>(TEXT("InventoryComponent"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	if (Health)
	{
		Health->OnDead.AddDynamic(this, &ATest_Character::CharacterDead);
	}

	if (InventoryComponent)
	{
		InventoryComponent->OnSwitchWeapon.AddDynamic(this, &ATest_Character::InitWeapon);
	}
	
}

void ATest_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetCurrentWeapon())
	{
		RotateWeaponTick(GetCurrentWeapon());
	}
}

EMovementState ATest_Character::GetMovementState()
{
	return MovementState;
}

void ATest_Character::CharacterDead()
{
	CharacterDead_BP();

	if (HasAuthority())
	{
		float TimeAnim = 0.0f;
		int8 Random = FMath::RandHelper(DeadAnim.Num());
		if (DeadAnim.IsValidIndex(Random) && DeadAnim[Random] && GetMesh() && GetMesh()->GetAnimInstance())
		{
			TimeAnim = DeadAnim[Random]->GetPlayLength();
			PlayAnim(DeadAnim[Random]);
		}

		if (GetController())
		{
			GetController()->UnPossess();
		}

		float DecreaseAniTimer = FMath::FRandRange(0.2f, 1.0f);

		GetWorldTimerManager().SetTimer(TimerHandle_RagdollTimer, this, &ATest_Character::EnableRagdoll,
									TimeAnim - DecreaseAniTimer, false);

		SetLifeSpan(20.0f);
		if (GetCurrentWeapon())
		{
			GetCurrentWeapon()->SetLifeSpan(20.0f);
		}
	}
	else
	{
		AttackEvent(false);
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void ATest_Character::EnableRagdoll()
{
	if (GetMesh())
	{
		GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
		GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
	}
}

float ATest_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Health && Health->GetIsAlive())
	{
		Health->ChangeHealthValue(-DamageAmount);
	}

	return ActualDamage;
}


void ATest_Character::CharacterUpdate()
{
	float ResultSpeed = 300.0f;
	switch (MovementState)
	{
	case EMovementState::Aim_State:
		ResultSpeed = CharacterSpeed.AimSpeed;
		break;
	case EMovementState::Walk_State:
		ResultSpeed = CharacterSpeed.WalkSpeed;
		break;
	case EMovementState::Sprint_State:
		ResultSpeed = CharacterSpeed.SprintSpeed;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResultSpeed;
}

void ATest_Character::ChangeMovementState()
{
	EMovementState NewState = EMovementState::Walk_State;

	ATest_PlayerController* MyController = Cast<ATest_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyController)
	{
		if (!MyController->bIsSprinting && !MyController->bIsAiming)
		{
			NewState = EMovementState::Walk_State;
		}
		else
		{
			if (MyController->bIsSprinting)
			{
				NewState = EMovementState::Sprint_State;
			}
			else
			{
				if (!MyController->bIsSprinting && MyController->bIsAiming)
				{
					NewState = EMovementState::Aim_State;
				}
			}
		}
	}

	SetMovementState(NewState);
}

void ATest_Character::AttackEvent(bool bIsFiring)
{
	if (Health && Health->GetIsAlive())
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

void ATest_Character::AimEvent(bool bIsAiming)
{
	if (bIsAiming)
	{
		ChangeMovementState();
		CameraBoom->TargetArmLength = 30.0f;
	}
	else
	{
		ChangeMovementState();
		CameraBoom->TargetArmLength = 100.0f;
	}
}

void ATest_Character::SprintEvent()
{
	ChangeMovementState();
}

void ATest_Character::InitWeapon(FName WeaponName, FAdditionalWeaponInfo AdditionalWeaponInfo, int32 NewCurrentIndexWeapon)
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
					
					MyWeapon->OnWeaponReloadStart.AddDynamic(this, &ATest_Character::WeaponReloadStart);
					MyWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATest_Character::WeaponReloadEnd);
					MyWeapon->OnWeaponFire.AddDynamic(this, &ATest_Character::WeaponFire);

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

void ATest_Character::TryReloadWeapon()
{
	if (Health && Health->GetIsAlive() && CurrentWeapon && !CurrentWeapon->WeaponReloading)
	{
		if (CurrentWeapon->GetWeaponRound() < CurrentWeapon->WeaponInfo.MaxRound && CurrentWeapon->CheckCanWeaponReload())
		{
			CurrentWeapon->InitReload();
		}
	}
}

void ATest_Character::WeaponFire(UAnimMontage* Anim)
{
	if (InventoryComponent && CurrentWeapon)
	{
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	}
	
	WeaponFire_BP(Anim);
}

void ATest_Character::WeaponReloadStart(UAnimMontage* Anim)
{
	WeaponReloadStart_BP(Anim);
}

void ATest_Character::WeaponReloadEnd(bool bIsSuccess, int32 AmmoSafe)
{
	if (InventoryComponent&&  CurrentWeapon)
	{
		InventoryComponent->AmmoSlotChangeValue(CurrentWeapon->WeaponInfo.WeaponType, AmmoSafe);
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	}

	WeaponReloadEnd_BP(bIsSuccess);
}

void ATest_Character::TrySwitchWeapon()
{
	int32 Index = GetCurrentWeaponIndex();
	if (Index == 0)
	{
		TrySwitchWeapon(1);
	}
	if (Index == 1)
	{
		TrySwitchWeapon(0);
	}
}

void ATest_Character::TrySwitchWeapon(int32 ToIndex)
{
	if (CurrentWeapon && !CurrentWeapon->WeaponReloading && InventoryComponent->WeaponSlots.IsValidIndex(ToIndex))
	{
		if (CurrentIndexWeapon != ToIndex && InventoryComponent)
		{
			int32 OldIndex = CurrentIndexWeapon;
			FAdditionalWeaponInfo OldInfo;

			if (CurrentWeapon)
			{
				OldInfo = CurrentWeapon->AdditionalWeaponInfo;
				if (CurrentWeapon->WeaponReloading)
				{
					CurrentWeapon->CancelReload();
				}
			}

			InventoryComponent->SwitchWeaponByIndex(ToIndex, OldIndex, OldInfo);
		}
	}
}

void ATest_Character::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
}

void ATest_Character::WeaponReloadEnd_BP_Implementation(bool bIsSuccess)
{
}

void ATest_Character::WeaponFire_BP_Implementation(UAnimMontage* Anim)
{
}

AWeaponDefault* ATest_Character::GetCurrentWeapon()
{
	return CurrentWeapon;
}

int32 ATest_Character::GetCurrentWeaponIndex()
{
	return CurrentIndexWeapon;
}

bool ATest_Character::GetIsAlive()
{
	bool Result = false;
	if (Health)
	{
		Result = Health->GetIsAlive();
	}

	return Result;
}


void ATest_Character::CharacterDead_BP_Implementation()
{
}

void ATest_Character::SetActorRotationByYaw(float Yaw)
{
	if (Controller && !Controller->IsLocalController())
	{
		SetActorRotation(FQuat(FRotator(0.0f, Yaw, 0.0f)));
	}
}

void ATest_Character::SetMovementState(EMovementState NewState)
{
	MovementState = NewState;
	CharacterUpdate();
}

void ATest_Character::PlayAnim(UAnimMontage* Anim)
{
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Anim);
	}
}

void ATest_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATest_Character::RotateWeaponTick(AWeaponDefault* Weapon)
{
	Weapon->SetActorRotation(FollowCamera->GetForwardVector().Rotation());
}

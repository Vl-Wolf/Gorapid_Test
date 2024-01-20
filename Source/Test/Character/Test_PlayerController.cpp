// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Character/Test_PlayerController.h"

#include "Test_Character.h"
#include "GameFramework/Character.h"
#include "Test/Game/Test_PlayerState.h"

void ATest_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATest_Character* MyCharacter = Cast<ATest_Character>(InPawn);
	if (MyCharacter)
	{
		ATest_PlayerState* MyPlayerState = GetPlayerState<ATest_PlayerState>();
		{
			MyCharacter->GetInventoryComponent()->InitInventory(MyPlayerState->WeaponSlot);
		}
	}
}

ATest_PlayerController::ATest_PlayerController()
{
	
}

void ATest_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Axis
	InputComponent->BindAxis("MoveForward", this, &ATest_PlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATest_PlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ATest_PlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ATest_PlayerController::LookUp);

	//Action
	InputComponent->BindAction("Jump", IE_Pressed, this, &ATest_PlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ATest_PlayerController::StopJump);
	InputComponent->BindAction("Aim", IE_Pressed, this, &ATest_PlayerController::AimPressed);
	InputComponent->BindAction("Aim", IE_Released, this, &ATest_PlayerController::AimReleased);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ATest_PlayerController::SprintPressed);
	InputComponent->BindAction("Sprint", IE_Released, this, &ATest_PlayerController::SprintReleased);
	InputComponent->BindAction("Reload", IE_Pressed, this, &ATest_PlayerController::Reload);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATest_PlayerController::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &ATest_PlayerController::FireReleased);
	InputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &ATest_PlayerController::ChangeWeapon);
	
}

void ATest_PlayerController::MoveForward(float value)
{
	if (value != 0.0f && GetPawn())
	{
		APawn* MyPawn = GetPawn();
		const FRotator PawnControlRotation = GetControlRotation();
		const FVector ForwardVector = FRotationMatrix(PawnControlRotation).GetScaledAxis(EAxis::X);
		MyPawn->AddMovementInput(ForwardVector, value);
	}
}

void ATest_PlayerController::MoveRight(float value)
{
	if (value != 0.0f && GetPawn())
	{
		APawn* MyPawn = GetPawn();
		const FRotator PawnControlRotation = GetControlRotation();
		const FVector RightVector = FRotationMatrix(PawnControlRotation).GetScaledAxis(EAxis::Y);
		MyPawn->AddMovementInput(RightVector, value);
	}
}

void ATest_PlayerController::Turn(float value)
{
	AddYawInput(value);
}

void ATest_PlayerController::LookUp(float value)
{
	AddPitchInput(value);
}

void ATest_PlayerController::Jump()
{
	if (GetPawn())
	{
		ACharacter* MyCharacter = Cast<ACharacter>(GetPawn());
		if (MyCharacter)
		{
			MyCharacter->Jump();
		}
	}
}

void ATest_PlayerController::StopJump()
{
	if (GetPawn())
	{
		ACharacter* MyCharacter = Cast<ACharacter>(GetPawn());
		if (MyCharacter)
		{
			MyCharacter->StopJumping();
		}
	}
}

void ATest_PlayerController::AimPressed()
{
	if (!bIsSprinting)
	{
		ATest_Character* MyCharacter = Cast<ATest_Character>(GetPawn());
		if (MyCharacter)
		{
			bIsAiming = true;
			MyCharacter->AimEvent(bIsAiming);
		}
	}
}

void ATest_PlayerController::AimReleased()
{
	ATest_Character* MyCharacter = Cast<ATest_Character>(GetPawn());
	if (MyCharacter)
	{
		bIsAiming = false;
		MyCharacter->AimEvent(bIsAiming);
	}
}

void ATest_PlayerController::SprintPressed()
{
	if (!bIsSprinting)
	{
		ATest_Character* MyCharacter = Cast<ATest_Character>(GetPawn());
		if (MyCharacter)
		{
			bIsSprinting = true;
			MyCharacter->SprintEvent();
			AimReleased();
			FireReleased();
		}
	}
}

void ATest_PlayerController::SprintReleased()
{
	ATest_Character* MyCharacter = Cast<ATest_Character>(GetPawn());
	if (MyCharacter)
	{
		bIsSprinting = false;
		MyCharacter->SprintEvent();
	}
}

void ATest_PlayerController::Reload()
{
	ATest_Character* MyCharacter = Cast<ATest_Character>(GetPawn());
	
	if (MyCharacter)
	{
		MyCharacter->TryReloadWeapon();
	}
}

void ATest_PlayerController::ChangeWeapon()
{
	ATest_Character* MyCharacter = Cast<ATest_Character>(GetPawn());
	
	if (MyCharacter)
	{
		MyCharacter->TrySwitchWeapon();
	}
}

void ATest_PlayerController::FirePressed()
{
	if (!bIsSprinting)
	{
		ATest_Character* MyCharacter = Cast<ATest_Character>(GetPawn());
		if (MyCharacter)
		{
			bIsFiring = true;
			MyCharacter->AttackEvent(bIsFiring);
		}
	}
}

void ATest_PlayerController::FireReleased()
{
	ATest_Character* MyCharacter = Cast<ATest_Character>(GetPawn());
	if (MyCharacter)
	{
		bIsFiring = false;
		MyCharacter->AttackEvent(bIsFiring);
	}
}

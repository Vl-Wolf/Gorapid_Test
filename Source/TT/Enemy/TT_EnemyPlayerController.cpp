// Fill out your copyright notice in the Description page of Project Settings.


#include "TT_EnemyPlayerController.h"

#include "TT_Enemy.h"
#include "TT/Game/TT_PlayerState.h"


// Sets default values
ATT_EnemyPlayerController::ATT_EnemyPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATT_EnemyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATT_EnemyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATT_Enemy* MyEnemy = Cast<ATT_Enemy>(InPawn);
	if (MyEnemy)
	{
		MyEnemy->GetInventoryComponent()->InitInventory(MyEnemy->WeaponSlot);
	}
}

// Called every frame
void ATT_EnemyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


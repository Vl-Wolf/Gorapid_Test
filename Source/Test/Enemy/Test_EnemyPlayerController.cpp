// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_EnemyPlayerController.h"

#include "Test_Enemy.h"
#include "Test/Game/Test_PlayerState.h"


// Sets default values
ATest_EnemyPlayerController::ATest_EnemyPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATest_EnemyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATest_EnemyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ATest_Enemy* MyEnemy = Cast<ATest_Enemy>(InPawn);
	if (MyEnemy)
	{
		MyEnemy->GetInventoryComponent()->InitInventory(MyEnemy->WeaponSlot);
	}
}

// Called every frame
void ATest_EnemyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


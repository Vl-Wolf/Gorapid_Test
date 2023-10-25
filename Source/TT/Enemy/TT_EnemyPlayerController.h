// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TT_EnemyPlayerController.generated.h"

UCLASS()
class TT_API ATT_EnemyPlayerController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATT_EnemyPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

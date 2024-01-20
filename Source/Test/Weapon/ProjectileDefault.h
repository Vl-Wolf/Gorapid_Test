// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Test/FuncLibrary/Types.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectileDefault.generated.h"

UCLASS()
class TEST_API AProjectileDefault : public AActor
{
	GENERATED_BODY()

public:
	
	AProjectileDefault();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Components)
	UStaticMeshComponent* BulletMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Components)
	USphereComponent* BulletCollisionSphere = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Components)
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UNiagaraComponent* BulletNiagara = nullptr;

	FProjectileInfo ProjectileInfo;

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitProjectile(FProjectileInfo InitParam);
	UFUNCTION()
	virtual void BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
											FVector NormalImpulse, const FHitResult &Hit);

	UFUNCTION()
	virtual void ImpactProjectile();

	UFUNCTION()
	void InitVisualMeshProjectile(UStaticMesh* NewMesh, FTransform MeshRelative);
	UFUNCTION()
	void InitVisualTrailProjectile(UNiagaraSystem* NewTrail, FTransform TrailRelative);
	UFUNCTION()
	void SpawnHitDecal(UMaterialInterface* DecalMaterial, UPrimitiveComponent* OtherComponent, FHitResult HitResult);
	UFUNCTION()
	void SpawnHitFX(UNiagaraSystem* FX, FHitResult HitResult);
	UFUNCTION()
	void SpawnHitSound(USoundBase* HitSound, FHitResult HitResult);
	UFUNCTION()
	void InitVelocity(float InitSpeed, float MaxSpeed);

	//virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;

	
};

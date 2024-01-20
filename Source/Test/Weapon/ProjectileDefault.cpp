// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDefault.h"

#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"
#include "NiagaraComponent.h"


// Sets default values
AProjectileDefault::AProjectileDefault()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	BulletCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	BulletCollisionSphere->SetSphereRadius(16.0f);
	BulletCollisionSphere->bReturnMaterialOnMove = true;
	BulletCollisionSphere->SetCanEverAffectNavigation(false);

	RootComponent = BulletCollisionSphere;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCanEverAffectNavigation(false);

	BulletNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagara"));
	BulletNiagara->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void AProjectileDefault::BeginPlay()
{
	Super::BeginPlay();

	BulletCollisionSphere->OnComponentHit.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereHit);
}

void AProjectileDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileDefault::InitProjectile(FProjectileInfo InitParam)
{
	this->SetLifeSpan(InitParam.ProjectileLifeTime);

	if (InitParam.ProjectileStaticMesh)
	{
		InitVisualMeshProjectile(InitParam.ProjectileStaticMesh, InitParam.ProjectileStaticMeshOffset);
	}
	else
	{
		BulletMesh->DestroyComponent();
	}

	if (InitParam.ProjectileTrailFX)
	{
		InitVisualTrailProjectile(InitParam.ProjectileTrailFX, InitParam.ProjectileTrailFXOffset);
	}
	else
	{
		BulletNiagara->DestroyComponent();
	}

	InitVelocity(InitParam.ProjectileInitSpeed, InitParam.ProjectileMaxSpeed);

	ProjectileInfo = InitParam;
}

void AProjectileDefault::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor && Hit.PhysMaterial.IsValid())
	{
		EPhysicalSurface MySurfaceType = UGameplayStatics::GetSurfaceType(Hit);

		if (ProjectileInfo.HitDecals.Contains(MySurfaceType))
		{
			UMaterialInterface* MyMaterial = ProjectileInfo.HitDecals[MySurfaceType];

			if (MyMaterial && OtherComp)
			{
				SpawnHitDecal(MyMaterial, OtherComp, Hit);
			}
		}

		if (ProjectileInfo.HitFXs.Contains(MySurfaceType))
		{
			UNiagaraSystem* MyParticle = ProjectileInfo.HitFXs[MySurfaceType];

			if (MyParticle)
			{
				SpawnHitFX(MyParticle, Hit);
			}
		}

		if (ProjectileInfo.HitSound)
		{
			SpawnHitSound(ProjectileInfo.HitSound, Hit);
		}
	}

	UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileInfo.ProjectileDamage, Hit.TraceStart, Hit,
											GetInstigatorController(), this, nullptr);

	UAISense_Damage::ReportDamageEvent(GetWorld(), Hit.GetActor(), GetInstigator(), ProjectileInfo.ProjectileDamage,
											Hit.Location, Hit.Location);

	ImpactProjectile();
}

void AProjectileDefault::ImpactProjectile()
{
	this->Destroy();
}

void AProjectileDefault::InitVisualMeshProjectile(UStaticMesh* NewMesh,
	FTransform MeshRelative)
{
	BulletMesh->SetStaticMesh(NewMesh);
	BulletMesh->SetRelativeTransform(MeshRelative);
}

void AProjectileDefault::InitVisualTrailProjectile(UNiagaraSystem* NewTrail,
	FTransform TrailRelative)
{
	
	BulletNiagara->SetAsset(NewTrail);
	BulletNiagara->SetRelativeTransform(TrailRelative);
}

void AProjectileDefault::SpawnHitDecal(UMaterialInterface* DecalMaterial,
	UPrimitiveComponent* OtherComponent, FHitResult HitResult)
{
	UGameplayStatics::SpawnDecalAttached(DecalMaterial, FVector(20.0f), OtherComponent, NAME_None,
									HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(),
									EAttachLocation::KeepWorldPosition, 10.0f);
}

void AProjectileDefault::SpawnHitFX(UNiagaraSystem* FX, FHitResult HitResult)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX, HitResult.ImpactPoint,
														HitResult.ImpactNormal.Rotation(), FVector(1.0f));
}

void AProjectileDefault::SpawnHitSound(USoundBase* HitSound, FHitResult HitResult)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.ImpactPoint);
}

void AProjectileDefault::InitVelocity(float InitSpeed, float MaxSpeed)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = GetActorForwardVector() * InitSpeed;
		ProjectileMovement->MaxSpeed = MaxSpeed;
		ProjectileMovement->InitialSpeed = InitSpeed;
	}
}

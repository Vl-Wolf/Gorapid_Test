// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDefault.h"

#include "DrawDebugHelpers.h"
#include "ProjectileDefault.h"
#include "Kismet/GameplayStatics.h"
#include "TT/Character/TT_InventoryComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AWeaponDefault::AWeaponDefault()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	SkeletalMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshWeapon->SetGenerateOverlapEvents(false);
	SkeletalMeshWeapon->SetCollisionProfileName(TEXT("NoCollisison"));
	SkeletalMeshWeapon->SetupAttachment(RootComponent);

	StaticMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshWeapon->SetGenerateOverlapEvents(false);
	StaticMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshWeapon->SetupAttachment(RootComponent);

	ShootLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootLocation"));
	ShootLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponDefault::BeginPlay()
{
	Super::BeginPlay();

	WeaponInit();
}

void AWeaponDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FireTick(DeltaTime);
		ReloadTick(DeltaTime);
	}
}

void AWeaponDefault::FireTick(float DeltaTime)
{
	if(WeaponFiring && GetWeaponRound() > 0 && !WeaponReloading)
	{
		if (FireTimer < 0.0f)
		{
			Fire();
		}
		else
		{
			FireTimer -= DeltaTime;
		}
	}
}

void AWeaponDefault::ReloadTick(float DeltaTime)
{
	if(WeaponReloading)
	{
		if (ReloadTimer < 0.0f)
		{
			FinishReload();
		}
		else
		{
			ReloadTimer -= DeltaTime;
		}
	}
}

void AWeaponDefault::WeaponInit()
{
	if (SkeletalMeshWeapon && !SkeletalMeshWeapon->GetSkeletalMeshAsset())
	{
		SkeletalMeshWeapon->DestroyComponent(true);
	}
	if (StaticMeshWeapon && !StaticMeshWeapon->GetStaticMesh())
	{
		StaticMeshWeapon->DestroyComponent();
	}
	
}

void AWeaponDefault::Fire()
{
	UAnimMontage* AnimPlay = nullptr;
	if (WeaponAiming)
	{
		AnimPlay = WeaponInfo.AnimationWeaponInfo.AnimCharacterFireAim;
	}
	else
	{
		AnimPlay = WeaponInfo.AnimationWeaponInfo.AnimCharacterFire;
	}

	if (WeaponInfo.AnimationWeaponInfo.AnimWeaponFire)
	{
		AnimWeaponStart(WeaponInfo.AnimationWeaponInfo.AnimWeaponFire);
	}
	
	FireTimer = WeaponInfo.RateOfFire;
	AdditionalWeaponInfo.Round -= 1;

	OnWeaponFire.Broadcast(AnimPlay);

	FXWeaponFire(WeaponInfo.WeaponFireEffect, WeaponInfo.SoundWeaponFire);

	if (ShootLocation)
	{
		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetForwardVector().Rotation();
		FProjectileInfo ProjectileInfo;
		ProjectileInfo = GetProjectile();
		FVector EndLocation = SpawnLocation + ShootLocation->GetForwardVector() * WeaponInfo.DistanceTrace;
		
		if (ProjectileInfo.Projectile)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParameters.Owner = GetOwner();
			SpawnParameters.Instigator = GetInstigator();

			AProjectileDefault* MyProjectile = Cast<AProjectileDefault>(GetWorld()->SpawnActor(ProjectileInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawnParameters));
			if (MyProjectile)
			{
				MyProjectile->InitProjectile(WeaponInfo.ProjectileInfo);
			}
		}
		else
		{
			FHitResult HitResult;
			TArray<AActor*> Actors;
			if (ShowDebug)
			{
				DrawDebugLine(GetWorld(), SpawnLocation, EndLocation, FColor::Black,
								false, 5.0f, '\000', 0.5f);
			}

			UKismetSystemLibrary::LineTraceSingle(GetWorld(), SpawnLocation, EndLocation,
													TraceTypeQuery4, false, Actors,
													EDrawDebugTrace::ForDuration,HitResult, true,
													FLinearColor::Red, FLinearColor::Green,
													5.0f);

			

			if (HitResult.GetActor() && HitResult.PhysMaterial.IsValid())
			{
				EPhysicalSurface MySurfaceType = UGameplayStatics::GetSurfaceType(HitResult);

				if (WeaponInfo.ProjectileInfo.HitDecals.Contains(MySurfaceType))
				{
					UMaterialInterface* MyMaterial = WeaponInfo.ProjectileInfo.HitDecals[MySurfaceType];

					if (MyMaterial && HitResult.GetComponent())
					{
						SpawnTraceHitDecal(MyMaterial, HitResult);
					}
				}

				if (WeaponInfo.ProjectileInfo.HitFXs.Contains(MySurfaceType))
				{
					UNiagaraSystem* MyParticle = WeaponInfo.ProjectileInfo.HitFXs[MySurfaceType];

					if (MyParticle)
					{
						SpawnTraceHitFX(MyParticle, HitResult);
					}
				}

				if (WeaponInfo.ProjectileInfo.HitSound)
				{
					SpawnTraceHitSound(WeaponInfo.ProjectileInfo.HitSound, HitResult);
				}

				UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), WeaponInfo.ProjectileInfo.ProjectileDamage,
													HitResult.TraceStart, HitResult, GetInstigatorController(),
													this, nullptr);
			}
		}
	}

	if (GetWeaponRound() <= 0 && !WeaponReloading)
	{
		if (CheckCanWeaponReload())
		{
			InitReload();
		}
	}
}

void AWeaponDefault::SetWeaponStateFire(bool bIsFire)
{
	if (CheckWeaponCanFire())
	{
		WeaponFiring = bIsFire;
	}
	else
	{
		WeaponFiring = false;
		FireTimer = 0.01f;
	}
}

bool AWeaponDefault::CheckWeaponCanFire()
{
	return !BlockFire;
}

FProjectileInfo AWeaponDefault::GetProjectile()
{
	return  WeaponInfo.ProjectileInfo;
}

int32 AWeaponDefault::GetWeaponRound()
{
	return AdditionalWeaponInfo.Round;
}

void AWeaponDefault::InitReload()
{
	WeaponReloading = true;

	ReloadTimer = WeaponInfo.ReloadTime;

	UAnimMontage* AnimPlay = nullptr;
	if (WeaponAiming)
	{
		AnimPlay = WeaponInfo.AnimationWeaponInfo.AnimCharacterReloadAnim;
	}
	else
	{
		AnimPlay = WeaponInfo.AnimationWeaponInfo.AnimCharacterReload;
	}

	OnWeaponReloadStart.Broadcast(AnimPlay);

	UAnimMontage* AnimWeaponPlay = nullptr;

	if (WeaponAiming)
	{
		AnimWeaponPlay = WeaponInfo.AnimationWeaponInfo.AnimWeaponReloadAnim;
	}
	else
	{
		AnimWeaponPlay = WeaponInfo.AnimationWeaponInfo.AnimWeaponReload;
	}

	if (WeaponInfo.AnimationWeaponInfo.AnimCharacterReload && SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
	{
		AnimWeaponStart(AnimWeaponPlay);
	}
}

void AWeaponDefault::FinishReload()
{
	WeaponReloading = false;

	int8 AvailableAmmoFromInventory = GetAvailableAmmoForReload();
	int8 AmmoNeedTakeFromInventory;
	int8 NeedToReload = WeaponInfo.MaxRound - AdditionalWeaponInfo.Round;

	if (NeedToReload > AvailableAmmoFromInventory)
	{
		AdditionalWeaponInfo.Round += AvailableAmmoFromInventory;
		AmmoNeedTakeFromInventory = AvailableAmmoFromInventory;
	}
	else
	{
		AdditionalWeaponInfo.Round += NeedToReload;
		AmmoNeedTakeFromInventory = NeedToReload;
	}

	OnWeaponReloadEnd.Broadcast(true, -AmmoNeedTakeFromInventory);
}

void AWeaponDefault::CancelReload()
{
	WeaponReloading = false;
	if (SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
	{
		SkeletalMeshWeapon->GetAnimInstance()->StopAllMontages(0.15f);
	}

	OnWeaponReloadEnd.Broadcast(false, 0);
}

bool AWeaponDefault::CheckCanWeaponReload()
{
	bool Result = true;

	if (GetOwner())
	{
		UTT_InventoryComponent* MyInventory = Cast<UTT_InventoryComponent>(GetOwner()->GetComponentByClass(UTT_InventoryComponent::StaticClass()));
		if (MyInventory)
		{
			int8 AvailableAmmoForWeapon;
			if (!MyInventory->CheckAmmoForWeapon(WeaponInfo.WeaponType, AvailableAmmoForWeapon))
			{
				Result = false;
			}
		}
	}
	
	return Result;
}

int8 AWeaponDefault::GetAvailableAmmoForReload()
{
	int8 AvailableAmmoForWeapon = WeaponInfo.MaxRound;

	if (GetOwner())
	{
		UTT_InventoryComponent* MyInventory = Cast<UTT_InventoryComponent>(GetOwner()->GetComponentByClass(UTT_InventoryComponent::StaticClass()));
		if (MyInventory)
		{
			if (MyInventory->CheckAmmoForWeapon(WeaponInfo.WeaponType, AvailableAmmoForWeapon))
			{
				AvailableAmmoForWeapon = AvailableAmmoForWeapon;
			}
		}
	}

	return AvailableAmmoForWeapon;
}

void AWeaponDefault::AnimWeaponStart(UAnimMontage* Anim)
{
	if (Anim && SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
	{
		SkeletalMeshWeapon->GetAnimInstance()->Montage_Play(Anim);
	}
}

void AWeaponDefault::FXWeaponFire(UNiagaraSystem* FXFire, USoundBase* SoundFire)
{
	if (SoundFire)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundFire, ShootLocation->GetComponentLocation());
	}
	if (FXFire)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FXFire, ShootLocation->GetComponentLocation(),
															ShootLocation->GetComponentRotation());
	}
}

void AWeaponDefault::SpawnTraceHitDecal(UMaterialInterface* DecalMaterial,
	FHitResult HitResult)
{
	UGameplayStatics::SpawnDecalAttached(DecalMaterial, FVector(20.0f), HitResult.GetComponent(), NAME_None,
											HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(),
											EAttachLocation::KeepWorldPosition, 10.0f);
}

void AWeaponDefault::SpawnTraceHitFX(UNiagaraSystem* FX, FHitResult HitResult)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX, HitResult.ImpactPoint,
														HitResult.ImpactNormal.Rotation(), FVector(1.0f));
}

void AWeaponDefault::SpawnTraceHitSound(USoundBase* HitSound, FHitResult HitResult)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.ImpactPoint);
}


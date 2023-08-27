// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Shooting3D.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

const FName MuzzleSoundKey = TEXT("MuzzleFlashSocket");

AGun::AGun()
	: Root           ( nullptr )
	, Mesh           ( nullptr )
	, MuzzleFlesh    ( nullptr )
	, ImpactHit      ( nullptr )
	, MaxRange       ( 2000.0f )
	, Damage         ( 10.0f   )
	, MuzzleSound    ( nullptr )
	, ImpactSound    ( nullptr )
	, EmptyAmmoSound ( nullptr )
	, AmmoChargeCount( 30      )
	, Ammo           ( 0       )
	, CurruntAmmoMax ( 30      )
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	if( UtilCommon::IsSafe( Mesh ) )
		Mesh->SetupAttachment(Root);

	Ammo = AmmoChargeCount;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::PullTrigger()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if ( UtilCommon::IsNull( OwnerPawn ) ) 
		return;

	AController* OwnerController = OwnerPawn->GetController();
	if ( UtilCommon::IsNull( OwnerController ) )
		return;
	
	if( OwnerPawn->IsPlayerControlled() )
	{
		if (Ammo <= 0) 
		{
			UGameplayStatics::SpawnSoundAttached( EmptyAmmoSound, Mesh, MuzzleSoundKey );
			return;
		}

		Ammo--;
	}
	
	UGameplayStatics::SpawnSoundAttached( MuzzleSound, Mesh, MuzzleSoundKey );
	UGameplayStatics::SpawnEmitterAttached( MuzzleFlesh, Mesh, MuzzleSoundKey );
	
	FVector  Lotation = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;

	OwnerController->GetPlayerViewPoint( Lotation, Rotation );

	FVector End = Lotation + Rotation.Vector() * MaxRange;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	auto* CurWorld = GetWorld();
	if (UtilCommon::IsNull(CurWorld))
		return;

	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Lotation, End, ECollisionChannel::ECC_GameTraceChannel1,Params);
	if (bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactHit, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		AActor* HitActor = Hit.GetActor();
		if ( UtilCommon::IsSafe( HitActor ) )
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

void AGun::AmmoReload()
{
	int32 EmptyAmmo = AmmoChargeCount - Ammo;
	if (CurruntAmmoMax < EmptyAmmo)
	{
		EmptyAmmo = CurruntAmmoMax;
	}

	Ammo += EmptyAmmo;
	CurruntAmmoMax -= EmptyAmmo;
}

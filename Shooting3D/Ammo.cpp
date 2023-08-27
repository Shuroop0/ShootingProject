// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"
#include "Shooting3D.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"

AAmmo::AAmmo()
	: Root      ( nullptr )
	, Mesh      ( nullptr )
	, RotateRate( 30.0f   )
	, AmmoCount ( 15      )
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (UtilCommon::IsSafe(Mesh))
	{
		Mesh->SetupAttachment(Root);
		Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::OnOverlapBegin);
	}
}

void AAmmo::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, RotateRate * DeltaTime, 0.f));
}

void AAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* Player = Cast<APawn>(OtherActor);
	if (UtilCommon::IsNull(Player))
		return;

	AShooterPlayerController* Controller = Cast<AShooterPlayerController>(Player->GetController());
	if (UtilCommon::IsNull(Controller))
		return;
	
	auto* ShooterCharcter = Cast<AShooterCharacter>(Player);
	if (UtilCommon::IsNull(ShooterCharcter))
		return;

	ShooterCharcter->AmmoCharge(AmmoCount);
	Destroy();
}
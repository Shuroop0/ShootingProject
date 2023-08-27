// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Shooting3D.h"

#include "Ammo.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ProgressBar.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Gun.h"
#include "SimpleShooterGameModeBase.h"
#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"


AShooterCharacter::AShooterCharacter()
	: GunClass      ( nullptr )
	, AmmoClass     ( nullptr )
	, Gun           ( nullptr )
	, AmmoActor     ( nullptr )
	, HpProgressBar ( nullptr )
	, HpBarWidget   ( nullptr )
	, RotationRate  ( 10.0f   )
	, HealthMax     ( 0.0f    )
	, Health        ( 0.0f    )
	, HealthTime    ( 5.0f    )
	, HealthCounter ( 0.0f    )
	, HitCount      ( 0.0f    )
	, HpBarViewDelay( 5.0f    )
	, bDeath        ( false   )
{
	PrimaryActorTick.bCanEverTick = true;

	HpBarWidget = CreateDefaultSubobject<UWidgetComponent>( TEXT("HpBarWidget" ) );
	if ( UtilCommon::IsSafe( HpBarWidget ) )
	{
		HpBarWidget->SetupAttachment( RootComponent );
	}
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitAttachGun();
	InitHpProgressBar();
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HealthRegain(DeltaTime);
	
	if (HpBarWidget == nullptr) return;
		HpBarView(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AShooterCharacter::Reload);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}


void AShooterCharacter::InitAttachGun()
{
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	if (UtilCommon::IsNull(Gun))
		return;

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

void AShooterCharacter::InitHpProgressBar()
{
	Health = HealthMax;

	auto* AIConteroller = Cast<AShooterAIController>(GetController());
	if (UtilCommon::IsNull(AIConteroller))
		return;

	if (UtilCommon::IsNull(HpBarWidget))
		return;

	HpBarWidget->SetVisibility(false);

	UUserWidget* HpBarUserWidget = HpBarWidget->GetUserWidgetObject();
	if (UtilCommon::IsNull(HpBarUserWidget))
		return;
	
	HpProgressBar = Cast<UProgressBar>(HpBarUserWidget->GetWidgetFromName( TEXT("Enemy_HP_ProgressBar" ) ) );
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector()*AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Reload()
{
	if ( UtilCommon::IsNull( Gun ) )
		return;

	Gun->AmmoReload();
}

void AShooterCharacter::HealthRegain(float DeltaTime)
{
	auto* CharacterController = GetController();
	if (UtilCommon::IsNull(CharacterController))
		return;

	APawn* CharacterPawn = CharacterController->GetPawn();
	if (UtilCommon::IsNull(CharacterPawn))
		return;

	if (CharacterPawn->IsPlayerControlled() == false)
		return;

	if (Health > HealthMax)
		return;

	if (bDeath)
		return;

	HealthCounter += DeltaTime;
	if (HealthCounter >= HealthTime)
	{
		Health++;
	}
}

void AShooterCharacter::AmmoAppear()
{
	auto* CurWorld = GetWorld();
	if (UtilCommon::IsNull(CurWorld))
		return;

	AmmoActor = CurWorld->SpawnActor<AAmmo>(AmmoClass);
	if (UtilCommon::IsSafe(AmmoActor))
	{
		AmmoActor->SetActorLocation( GetActorLocation() );
	}
}

float AShooterCharacter::GetHPPercent() const
{
	return Health / HealthMax;
}

FString AShooterCharacter::GetAmmo() const
{
	if (UtilCommon::IsNull(Gun))
		return TEXT("");

	return FString::Printf( TEXT("Ammo  %d/%d"), Gun->GetAmmo(), Gun->GetAmmoMax() );
}

void AShooterCharacter::Shoot()
{
	if (UtilCommon::IsNull(Gun))
		return;

	Gun->PullTrigger();
}

void AShooterCharacter::AmmoCharge(const int32 AmmoCount)
{
	if (UtilCommon::IsNull(Gun))
		return;

	Gun->ChargeAmmoMax(AmmoCount);
}



float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = 	Super::TakeDamage( DamageAmount,   DamageEvent,  EventInstigator,  DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("HP : %f.0f"), Health);

	HealthCounter = 0.0f;

	RefreshHpBar();

	if ( IsDead() )
	{
		Death();
	}

	return DamageToApply;
}

void AShooterCharacter::RefreshHpBar()
{
	auto* AIController = Cast<AShooterAIController>(GetController());
	if (UtilCommon::IsNull(AIController))
		return;

	if (UtilCommon::IsSafe(HpProgressBar))
	{
		HpProgressBar->SetPercent(GetHPPercent());
	}

	if (UtilCommon::IsSafe(HpBarWidget))
	{
		HpBarWidget->SetVisibility(true);
	}
		
	HitCount = 0.0f;
}

void AShooterCharacter::Death()
{
	bDeath = true;

	auto* AIController = Cast<AShooterAIController>(GetController());
	if (UtilCommon::IsSafe(AIController))
	{
		AmmoAppear();

		if (UtilCommon::IsSafe(HpBarWidget))
		{
			HpBarWidget->SetVisibility(false);
		}
	}

	auto* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	if ( UtilCommon::IsSafe( GameMode ) )
	{
		GameMode->PawnKilled( this );
	}

	DetachFromControllerPendingDestroy();

	auto* CapsuleComp = GetCapsuleComponent();
	if ( UtilCommon::IsSafe( CapsuleComp ) )
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AShooterCharacter::HpBarView(float DeltaTime)
{
	if (!HpBarWidget->IsVisible())return;
	
	HitCount += DeltaTime;
	if (HitCount >= HpBarViewDelay)
	{
		HitCount = 0.f;
		HpBarWidget->SetVisibility(false);
	}
}




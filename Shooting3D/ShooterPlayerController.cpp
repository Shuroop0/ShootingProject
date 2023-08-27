// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Shooting3D.h"

#include "Blueprint/UserWidget.h"
#include "TimerManager.h"


AShooterPlayerController::AShooterPlayerController()
	: LoseScreenClass ( nullptr )
	, WinScreenClass  ( nullptr )
	, HUDClass        ( nullptr )
	, HUD             ( nullptr )
	, RestartDelay    ( 5.0f    )
{
	PrimaryActorTick.bCanEverTick = false;
}
void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD = CreateWidget(this, HUDClass);
	if (UtilCommon::IsSafe(HUD))
	{
		HUD->AddToViewport();
	}
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	
	if (UtilCommon::IsSafe(HUD))
	{
		HUD->RemoveFromViewport();
	}
	
	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if ( UtilCommon::IsSafe(WinScreen) )
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (UtilCommon::IsSafe(LoseScreen))
		{
			LoseScreen->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
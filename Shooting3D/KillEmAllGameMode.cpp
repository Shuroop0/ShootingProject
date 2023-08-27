// Fill out your copyright notice in the Description page of Project Settings.

#include "EngineUtils.h"
#include "Shooting3D.h"

#include "ShooterAIController.h"
#include "GameFramework/Controller.h"
#include "KillEmAllGameMode.h"
#include "Kismet/GameplayStatics.h"


AKillEmAllGameMode::AKillEmAllGameMode()
	: BGM ( nullptr )
{
}

void AKillEmAllGameMode::BeginPlay()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BGM);
}

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	
	if ( UtilCommon::IsNull( PawnKilled ) )
		return;

	APlayerController* PlayerController = Cast<APlayerController>( PawnKilled->GetController() );
	if ( UtilCommon::IsSafe( PlayerController ) )
	{
		EndGame(false);
	}

	for ( auto* AIController : TActorRange<AShooterAIController>( GetWorld() ) )
	{
		if ( UtilCommon::IsNull( AIController ) )
			continue;

		if ( AIController->IsDead() == false )
			return; 
	}
	
	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for ( AController* Controller : TActorRange<AController>( GetWorld() ) )
	{
		if ( UtilCommon::IsNull( Controller ) )
			continue;

		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded( Controller->GetPawn(), bIsWinner );
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Shooting3D.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

const FName PlayerLocationParamName = TEXT("PlayerLocation");
const FName StartLocationParamName = TEXT("StartLocation");

AShooterAIController::AShooterAIController()
	: AIBehavior ( nullptr )
	, bDamage    ( false   )
{
	PrimaryActorTick.bCanEverTick = false;
}
void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	InitBlackboard();
}

void AShooterAIController::InitBlackboard()
{
	if ( AIBehavior == nullptr )
		return;

	RunBehaviorTree(AIBehavior);

	auto* BlackboardComponent = GetBlackboardComponent();
	if ( UtilCommon::IsNull (BlackboardComponent ) )
		return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if ( UtilCommon::IsSafe( PlayerPawn ) )
	{
		BlackboardComponent->SetValueAsVector( PlayerLocationParamName, PlayerPawn->GetActorLocation() );
	}

	APawn* AIOwnerPawn = GetPawn();
	if ( UtilCommon::IsSafe( AIOwnerPawn ) )
	{
		BlackboardComponent->SetValueAsVector( StartLocationParamName, AIOwnerPawn->GetActorLocation() );
	}
}

bool AShooterAIController::IsDead()
{
	auto* CharacterPawn = Cast<AShooterCharacter>( GetPawn() );
	if ( UtilCommon::IsSafe(CharacterPawn) )
	{
		return CharacterPawn->IsDead();
	}

	return true;
}
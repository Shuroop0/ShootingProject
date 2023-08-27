// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "Shooting3D.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = "Update Player Location If Seen";
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if ( UtilCommon::IsNull( AIController ) )
		return;

	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if ( UtilCommon::IsNull( BlackboardComponent ) )
		return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn( GetWorld(), 0 );

	if ( AIController->LineOfSightTo( PlayerPawn ) )
	{
		BlackboardComponent->SetValueAsObject( GetSelectedBlackboardKey(), PlayerPawn );
		FVector PlayerLocation = FVector::ZeroVector;
		if ( UtilCommon::IsSafe( PlayerPawn ) )
		{
			PlayerLocation = PlayerPawn->GetActorLocation();
		}

		BlackboardComponent->SetValueAsVector( "LastLocation", PlayerLocation );
	}
	else
	{
		BlackboardComponent->ClearValue( GetSelectedBlackboardKey() );
	}
}

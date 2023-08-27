// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "Shooting3D.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = "Update Player Location";
}


void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if ( UtilCommon::IsNull( PlayerPawn ) )
		return;

	auto* BlackBoardComponent = OwnerComp.GetBlackboardComponent();
	if( UtilCommon::IsSafe( BlackBoardComponent) )
	{
		BlackBoardComponent->SetValueAsVector( GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation() );
	}
}
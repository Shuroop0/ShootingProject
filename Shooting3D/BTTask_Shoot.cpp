// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "Shooting3D.h"

#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if ( UtilCommon::IsNull( AIOwner ) )
		return EBTNodeResult::Failed;;
	
	auto* Character = Cast<AShooterCharacter>( AIOwner->GetPawn() );
	if ( UtilCommon::IsNull(Character) )
		return EBTNodeResult::Failed;
	
	Character->Shoot();
	 
	return EBTNodeResult::Succeeded;
}

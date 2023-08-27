// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CelarBlackboardValue.h"
#include "Shooting3D.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CelarBlackboardValue::UBTTask_CelarBlackboardValue()
{
	NodeName = "Clear BlackboardValue";
}

EBTNodeResult::Type UBTTask_CelarBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if ( UtilCommon::IsNull( BlackboardComponent ) )
		return EBTNodeResult::Failed;

	BlackboardComponent->ClearValue( GetSelectedBlackboardKey() );

	return EBTNodeResult::Succeeded;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class UBehaviorTree;

UCLASS()
class SHOOTING3D_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AShooterAIController();

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;

	UPROPERTY()
	bool bDamage;

protected:
	virtual void BeginPlay() override;
	
	void InitBlackboard();

public:
	bool IsDead();
};

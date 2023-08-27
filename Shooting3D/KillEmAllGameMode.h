// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING3D_API AKillEmAllGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()
public:
	AKillEmAllGameMode();

private:
	UPROPERTY(EditAnywhere)
	USoundBase* BGM;

public:
	virtual void PawnKilled(APawn* PawnKilled) override;

private:
	void EndGame(bool bIsPlayerWinner);

protected:
	virtual void BeginPlay() override;
};

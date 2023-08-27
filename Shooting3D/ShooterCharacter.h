// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;
class AAmmo;
class UProgressBar;
class UWidgetComponent;

UCLASS()
class SHOOTING3D_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAmmo> AmmoClass;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY()
	AAmmo* AmmoActor;

	UPROPERTY()
	UProgressBar* HpProgressBar;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* HpBarWidget;

	UPROPERTY(EditAnywhere)
	float RotationRate;

	UPROPERTY(EditDefaultsOnly)
	float HealthMax;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	float HealthTime;

	UPROPERTY(VisibleAnywhere)
	float HealthCounter;

	UPROPERTY()
	float HitCount;

	UPROPERTY(EditAnywhere)
	float HpBarViewDelay;

	UPROPERTY()
	bool bDeath;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool IsDead()const;
	
	UFUNCTION(BlueprintPure)
	float GetHPPercent()const;
	
	UFUNCTION(BlueprintPure)
	FString GetAmmo() const;

	void Shoot();
	void AmmoCharge(const int32 AmmoCount);

protected:
	void InitAttachGun();
	void InitHpProgressBar();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	void Reload();
	void HealthRegain(float DeltaTime);
	void AmmoAppear();
	
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void RefreshHpBar();
	void Death();
	void HpBarView(float DeltaTime);
};

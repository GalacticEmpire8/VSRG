// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "SMGAttack.generated.h"

UCLASS()
class VSRG_API USMGAttack : public UAttackBase
{
	GENERATED_BODY()

public:
	virtual void InitializeAttack() override;

	virtual void ExecuteAttack(AMainCharacter* instigatorCharacter, FVector dir) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float sprayAngle = 10.f;

private:
	void FireBurstProjectile();

	FTimerHandle burstTimerHandle;
	int32 bulletsFired = 0;
	int32 maxBullets;
	float timeBetweenBullets = 0.1f;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "AssaultRifleAttack.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VSRG_API UAssaultRifleAttack : public UAttackBase
{
	GENERATED_BODY()

public:
	void initializeAttack();

	virtual void executeAttack_Implementation(AMainCharacter* instigatorCharacter) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float sprayAngle = 5.f;

private:
	void FireBurstProjectile();

	FTimerHandle burstTimerHandle;
	int32 bulletsFired = 0;
	int32 maxBullets;
	float timeBetweenBullets = 0.1f;
};

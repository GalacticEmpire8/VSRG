// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "PistolAttack.generated.h"

UCLASS()
class VSRG_API UPistolAttack : public UAttackBase
{
	GENERATED_BODY()

public:
	UPistolAttack();

	void initializeAttack();

	virtual void executeAttack_Implementation(AMainCharacter* instigatorCharacter) override;

private:
	void FireBurstProjectile();

	FTimerHandle burstTimerHandle;
	int32 bulletsFired = 0;
	int32 maxBullets;
	float timeBetweenBullets = 0.1f;

};

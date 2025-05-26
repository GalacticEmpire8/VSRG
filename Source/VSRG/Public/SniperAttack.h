// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "SniperAttack.generated.h"

/**
 * 
 */
UCLASS()
class VSRG_API USniperAttack : public UAttackBase
{
	GENERATED_BODY()

private:
	float critChance;
	float critMultiplier;
	
public:
	virtual void InitializeAttack() override;

	virtual void ExecuteAttack(AMainCharacter* instigatorCharacter, FVector dir) override;

	virtual void LevelUp() override;
};

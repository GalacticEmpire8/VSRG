// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "ShotgunAttack.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VSRG_API UShotgunAttack : public UAttackBase
{
	GENERATED_BODY()

private:
	float coneHalfAngleDegrees;

public:
	virtual void InitializeAttack() override;

	virtual void LevelUp() override;

	virtual void ExecuteAttack(AMainCharacter* instigatorCharacter, FVector dir) override;
};

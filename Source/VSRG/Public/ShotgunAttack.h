// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "ShotgunAttack.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VSRG_API UShotgunAttack : public UAttackBase
{
	GENERATED_BODY()

public:
	void initializeAttack();

	void levelUp();

	virtual void executeAttack_Implementation(AMainCharacter* instigatorCharacter) override;

	float coneHalfAngleDegrees = 40.0f;
};

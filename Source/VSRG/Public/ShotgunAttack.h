// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "ShotgunAttack.generated.h"

UCLASS()
class VSRG_API UShotgunAttack : public UAttackBase
{
	GENERATED_BODY()

public:
	UShotgunAttack();

	void initializeAttack();

	virtual void executeAttack_Implementation(AMainCharacter* instigatorCharacter) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PassiveItem.h"
#include "ArtificialHeart.generated.h"

/**
 * 
 */
UCLASS()
class VSRG_API UArtificialHeart : public UPassiveBase
{
	GENERATED_BODY()
	
public:
	virtual void Apply(AMainCharacter* Character) override;
	virtual void Upgrade(AMainCharacter* Character) override;
};

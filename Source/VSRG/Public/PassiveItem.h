// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PassiveItem.generated.h"

class AMainCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class VSRG_API UPassiveBase : public UItem
{
    GENERATED_BODY()
public:
    virtual void Apply(AMainCharacter* Character);
    virtual void Upgrade(AMainCharacter* Character);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "WeaponDataRow.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct VSRG_API FItemDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName itemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UItem> itemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 rarity;

};
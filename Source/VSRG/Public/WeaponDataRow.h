// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "WeaponDataRow.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct VSRG_API FWeaponDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UAttackBase> WeaponClass;

};
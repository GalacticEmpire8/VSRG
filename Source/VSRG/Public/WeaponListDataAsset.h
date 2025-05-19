// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackBase.h"
#include "WeaponListDataAsset.generated.h"

UCLASS()
class VSRG_API UWeaponListDataAsset : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSubclassOf<UAttackBase>> WeaponClasses;
};
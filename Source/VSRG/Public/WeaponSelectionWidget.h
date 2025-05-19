// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttackBase.h"
#include "WeaponSelectionWidget.generated.h"

UCLASS()
class VSRG_API UWeaponSelectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void InitWeaponOptions(const TArray<UAttackBase*>& Options);

    UFUNCTION(BlueprintImplementableEvent)
    void OnWeaponSelected(UAttackBase* SelectedWeapon);

    UPROPERTY(BlueprintReadOnly)
    TArray<UAttackBase*> WeaponOptions;
};
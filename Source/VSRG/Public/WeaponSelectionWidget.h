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
    // Initializes the widget with the given weapon options
    UFUNCTION(BlueprintCallable)
    void InitWeaponOptions(const TArray<UAttackBase*>& Options);

    // Called when a weapon is selected
    UFUNCTION(BlueprintCallable)
    virtual void OnWeaponSelected(UAttackBase* SelectedWeapon);

    // The weapon options to display
    UPROPERTY(BlueprintReadOnly)
    TArray<UAttackBase*> WeaponOptions;

    // The weapon the player selected
    UPROPERTY(BlueprintReadOnly)
    UAttackBase* SelectedWeapon;
};
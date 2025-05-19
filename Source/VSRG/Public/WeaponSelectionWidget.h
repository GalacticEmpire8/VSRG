// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttackBase.h"
#include <Components/Button.h>
#include "Components/TextBlock.h"
#include "WeaponSelectionWidget.generated.h"


UCLASS()
class VSRG_API UWeaponSelectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void InitWeaponOptions(const TArray<TSubclassOf<UAttackBase>>& Options);

    UFUNCTION(BlueprintCallable)
    virtual void OnWeaponSelected(TSubclassOf<UAttackBase> SelectedWeaponClass);

    UPROPERTY(BlueprintReadOnly)
    TArray<TSubclassOf<UAttackBase>> WeaponOptions;

    UPROPERTY(BlueprintReadOnly)
    TSubclassOf<UAttackBase> SelectedWeaponClass;

    // Button and text references (BindWidget)
    UPROPERTY(meta = (BindWidget))
    class UButton* WeaponButton1;
    UPROPERTY(meta = (BindWidget))
    class UButton* WeaponButton2;
    UPROPERTY(meta = (BindWidget))
    class UButton* WeaponButton3;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* WeaponText1;
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* WeaponText2;
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* WeaponText3;

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleWeaponButton1Clicked();
    UFUNCTION()
    void HandleWeaponButton2Clicked();
    UFUNCTION()
    void HandleWeaponButton3Clicked();
};
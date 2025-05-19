// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSelectionWidget.h"

void UWeaponSelectionWidget::InitWeaponOptions(const TArray<UAttackBase*>& Options)
{
    WeaponOptions = Options;
}

void UWeaponSelectionWidget::OnWeaponSelected(UAttackBase* InSelectedWeapon)
{
    SelectedWeapon = InSelectedWeapon;

    // Example: Hide the widget after selection
    RemoveFromParent();

    // TODO: Grant the weapon to the player here, or notify the owning character/game mode
    // e.g., if you have a delegate or want to call a function on the player, do it here.
}
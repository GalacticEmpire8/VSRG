// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSelectionWidget.h"

void UWeaponSelectionWidget::InitWeaponOptions(const TArray<UAttackBase*>& Options)
{
    WeaponOptions = Options;
}
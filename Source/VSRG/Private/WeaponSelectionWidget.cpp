// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSelectionWidget.h"
#include "MainCharacter.h"

void UWeaponSelectionWidget::InitWeaponOptions(const TArray<TSubclassOf<UAttackBase>>& Options)
{
    WeaponOptions = Options;

    if (WeaponText1 && WeaponOptions.IsValidIndex(0) && WeaponOptions[0])
    {
        UAttackBase* DefaultObj = WeaponOptions[0]->GetDefaultObject<UAttackBase>();
        WeaponText1->SetText(DefaultObj->weaponName);
    }
    if (WeaponText2 && WeaponOptions.IsValidIndex(1) && WeaponOptions[1])
    {
        UAttackBase* DefaultObj = WeaponOptions[1]->GetDefaultObject<UAttackBase>();
        WeaponText2->SetText(DefaultObj->weaponName);
    }
    if (WeaponText3 && WeaponOptions.IsValidIndex(2) && WeaponOptions[2])
    {
        UAttackBase* DefaultObj = WeaponOptions[2]->GetDefaultObject<UAttackBase>();
        WeaponText3->SetText(DefaultObj->weaponName);
    }
}

void UWeaponSelectionWidget::OnWeaponSelected(TSubclassOf<UAttackBase> InSelectedWeaponClass)
{
    SelectedWeaponClass = InSelectedWeaponClass;

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AMainCharacter* MC = Cast<AMainCharacter>(PC->GetPawn()))
        {
            MC->GrantWeapon(InSelectedWeaponClass);
        }

        PC->SetPause(false);
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }

    RemoveFromParent();
}

void UWeaponSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (WeaponButton1)
    {
        WeaponButton1->OnClicked.AddDynamic(this, &UWeaponSelectionWidget::HandleWeaponButton1Clicked);
    }
    if (WeaponButton2)
    {
        WeaponButton2->OnClicked.AddDynamic(this, &UWeaponSelectionWidget::HandleWeaponButton2Clicked);
    }
    if (WeaponButton3)
    {
        WeaponButton3->OnClicked.AddDynamic(this, &UWeaponSelectionWidget::HandleWeaponButton3Clicked);
    }
}

void UWeaponSelectionWidget::HandleWeaponButton1Clicked()
{
    if (WeaponOptions.IsValidIndex(0))
    {
        OnWeaponSelected(WeaponOptions[0]);
    }
}

void UWeaponSelectionWidget::HandleWeaponButton2Clicked()
{
    if (WeaponOptions.IsValidIndex(1))
    {
        OnWeaponSelected(WeaponOptions[1]);
    }
}

void UWeaponSelectionWidget::HandleWeaponButton3Clicked()
{
    if (WeaponOptions.IsValidIndex(2))
    {
        OnWeaponSelected(WeaponOptions[2]);
    }
}
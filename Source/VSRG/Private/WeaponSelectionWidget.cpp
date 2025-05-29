// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSelectionWidget.h"
#include "MainCharacter.h"

void UWeaponSelectionWidget::InitWeaponOptions(const TArray<TSubclassOf<UItem>>& Options)
{
    WeaponOptions = Options;

    // Option 1
    if (WeaponText1 && WeaponButton1)
    {
        if (WeaponOptions.IsValidIndex(0) && WeaponOptions[0])
        {
            UItem* DefaultObj = WeaponOptions[0]->GetDefaultObject<UItem>();
            WeaponText1->SetText(DefaultObj->itemName);
            WeaponButton1->SetVisibility(ESlateVisibility::Visible);
            WeaponText1->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            WeaponButton1->SetVisibility(ESlateVisibility::Collapsed);
            WeaponText1->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    // Option 2
    if (WeaponText2 && WeaponButton2)
    {
        if (WeaponOptions.IsValidIndex(1) && WeaponOptions[1])
        {
            UItem* DefaultObj = WeaponOptions[1]->GetDefaultObject<UItem>();
            WeaponText2->SetText(DefaultObj->itemName);
            WeaponButton2->SetVisibility(ESlateVisibility::Visible);
            WeaponText2->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            WeaponButton2->SetVisibility(ESlateVisibility::Collapsed);
            WeaponText2->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    // Option 3
    if (WeaponText3 && WeaponButton3)
    {
        if (WeaponOptions.IsValidIndex(2) && WeaponOptions[2])
        {
            UItem* DefaultObj = WeaponOptions[2]->GetDefaultObject<UItem>();
            WeaponText3->SetText(DefaultObj->itemName);
            WeaponButton3->SetVisibility(ESlateVisibility::Visible);
            WeaponText3->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            WeaponButton3->SetVisibility(ESlateVisibility::Collapsed);
            WeaponText3->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UWeaponSelectionWidget::OnWeaponSelected(TSubclassOf<UItem> InSelectedWeaponClass)
{
    SelectedWeaponClass = InSelectedWeaponClass;

    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AMainCharacter* MC = Cast<AMainCharacter>(PC->GetPawn()))
        {
            if (InSelectedWeaponClass->IsChildOf(UAttackBase::StaticClass()))
            {
                MC->GrantWeapon(InSelectedWeaponClass);
            }
            else if (InSelectedWeaponClass->IsChildOf(UPassiveBase::StaticClass()))
            {
                MC->GrantPassive(InSelectedWeaponClass);
            }
            // Optionally handle other item types here
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
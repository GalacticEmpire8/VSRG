// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSelectionWidget.h"
#include "MainCharacter.h"

void UWeaponSelectionWidget::InitWeaponOptions(
    const TArray<TSubclassOf<UItem>>& Options,
    const TArray<int32>& OptionLevels
)
{
    WeaponOptions = Options;

    // Option 1
    if (WeaponText1 && WeaponButton1)
    {
        if (WeaponOptions.IsValidIndex(0) && WeaponOptions[0])
        {
            UItem* DefaultObj = WeaponOptions[0]->GetDefaultObject<UItem>();
            int32 Level = OptionLevels[0];
            WeaponText1->SetText(DefaultObj->itemName);
            WeaponDesc1->SetText(DefaultObj->itemDesc[Level - 1]);
            WeaponButton1->SetVisibility(ESlateVisibility::Visible);
            WeaponText1->SetVisibility(ESlateVisibility::Visible);
            WeaponDesc1->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            WeaponButton1->SetVisibility(ESlateVisibility::Collapsed);
            WeaponText1->SetVisibility(ESlateVisibility::Collapsed);
            WeaponDesc1->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    // Option 2
    if (WeaponText2 && WeaponButton2)
    {
        if (WeaponOptions.IsValidIndex(1) && WeaponOptions[1])
        {
            UItem* DefaultObj = WeaponOptions[1]->GetDefaultObject<UItem>();
            int32 Level = OptionLevels[1];
            WeaponText2->SetText(DefaultObj->itemName);
            WeaponDesc2->SetText(DefaultObj->itemDesc[Level - 1]);
            WeaponButton2->SetVisibility(ESlateVisibility::Visible);
            WeaponText2->SetVisibility(ESlateVisibility::Visible);
            WeaponDesc2->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            WeaponButton2->SetVisibility(ESlateVisibility::Collapsed);
            WeaponText2->SetVisibility(ESlateVisibility::Collapsed);
            WeaponDesc2->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    // Option 3
    if (WeaponText3 && WeaponButton3)
    {
        if (WeaponOptions.IsValidIndex(2) && WeaponOptions[2])
        {
            UItem* DefaultObj = WeaponOptions[2]->GetDefaultObject<UItem>();
            int32 Level = OptionLevels[2];
            WeaponText3->SetText(DefaultObj->itemName);
            WeaponDesc3->SetText(DefaultObj->itemDesc[Level - 1]);
            WeaponButton3->SetVisibility(ESlateVisibility::Visible);
            WeaponText3->SetVisibility(ESlateVisibility::Visible);
            WeaponDesc3->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            WeaponButton3->SetVisibility(ESlateVisibility::Collapsed);
            WeaponText3->SetVisibility(ESlateVisibility::Collapsed);
            WeaponDesc3->SetVisibility(ESlateVisibility::Collapsed);
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
            UE_LOG(LogTemp, Warning, TEXT("Selected class: %s"), *InSelectedWeaponClass->GetName());
            if (InSelectedWeaponClass->IsChildOf(UAttackBase::StaticClass()))
            {
                UE_LOG(LogTemp, Warning, TEXT("Calling GrantWeapon"));
                MC->GrantWeapon(InSelectedWeaponClass);
            }
            else if (InSelectedWeaponClass->IsChildOf(UPassiveBase::StaticClass()))
            {
                UE_LOG(LogTemp, Warning, TEXT("Calling GrantPassive"));
                MC->GrantPassive(InSelectedWeaponClass);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Selected class is not a weapon or passive!"));
            }
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
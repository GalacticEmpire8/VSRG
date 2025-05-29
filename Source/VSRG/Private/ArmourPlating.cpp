// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmourPlating.h"
#include "MainCharacter.h"

void UArmourPlating::Apply(AMainCharacter* Character)
{
    if (Character)
    {
        Character->defense += 5.0f;
    }
}

void UArmourPlating::Upgrade(AMainCharacter* Character)
{
    if (Character && level <= 6)
    {
        level++;
        Character->defense += 5.0f;
    }
}


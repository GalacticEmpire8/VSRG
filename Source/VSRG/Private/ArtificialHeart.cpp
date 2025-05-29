// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtificialHeart.h"
#include "MainCharacter.h"

void UArtificialHeart::Apply(AMainCharacter* Character)
{
    if (Character)
    {
        Character->healthComponent->MaxHealth += 10;
        Character->healthComponent->Health += 10;
    }
}

void UArtificialHeart::Upgrade(AMainCharacter* Character)
{
    if (Character && level <= 6)
    {
        level++;
        Character->healthComponent->MaxHealth += 10;
        Character->healthComponent->Health += 10;
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PassiveItem.h"

UPassiveBase::UPassiveBase() {
	level = 1;
}

void UPassiveBase::Apply(AMainCharacter* Character) {
	// Default implementation does nothing, can be overridden in derived classes
}

void UPassiveBase::Upgrade(AMainCharacter* Character) {
	// Default implementation does nothing, can be overridden in derived classes
}
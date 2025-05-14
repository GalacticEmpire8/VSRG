// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBase.h"

void UAttackBase::executeAttack_Implementation(AMainCharacter* instigatorCharacter)
{
	if (!instigatorCharacter) return;
	owningCharacter = instigatorCharacter;

	usesLeft--;

	if (usesLeft <= 0) {
		cooldownLeft = cooldown;
		isOnCooldown = true;
	}
}

void UAttackBase::initializeAttack()
{
	// Initialize the attack properties based on the level
	damageRow = DamageData.GetRow<FWeaponData>(TEXT("DamageData Context"));
	if (damageRow) damage = damageRow->Level1;
	else UE_LOG(LogTemp, Warning, TEXT("DamageData Context not found"));

	cooldownRow = CooldownData.GetRow<FWeaponData>("");
	if (cooldownRow) cooldown = cooldownRow->Level1;
	else UE_LOG(LogTemp, Warning, TEXT("CooldownData not found"));

	projectileRow = ProjectileData.GetRow<FWeaponData>("");
	if (projectileRow) projectiles = projectileRow->Level1;
	else UE_LOG(LogTemp, Warning, TEXT("ProjectileData not found"));

	pierceRow = PierceData.GetRow<FWeaponData>("");
	if (pierceRow) pierce = pierceRow->Level1;
	else UE_LOG(LogTemp, Warning, TEXT("PierceData not found"));

	usesRow = UsesData.GetRow<FWeaponData>("");
	if (usesRow) uses = usesRow->Level1;
	else UE_LOG(LogTemp, Warning, TEXT("UsesData not found"));

	rangeRow = RangeData.GetRow<FWeaponData>("");
	if (rangeRow) range = rangeRow->Level1;
	else range = 1000.f; // Default value if not found
}

void UAttackBase::onBeat() {
	if (isOnCooldown) {
		cooldownLeft--;
		UE_LOG(LogTemp, Warning, TEXT("Cooldown left: %d"), cooldownLeft);
	}

	if (cooldownLeft <= 0) {
		cooldownLeft = 0;
		usesLeft = uses;
		isOnCooldown = false;
	}
}
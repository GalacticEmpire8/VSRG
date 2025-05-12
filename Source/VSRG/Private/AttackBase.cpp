// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBase.h"

// Default empty implementation
void UAttackBase::executeAttack_Implementation(AMainCharacter* instigatorCharacter)
{

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
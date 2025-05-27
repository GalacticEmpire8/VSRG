// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBase.h"

UAttackBase::UAttackBase() {
	level = 1;
	owningCharacter = nullptr;
}

void UAttackBase::ExecuteAttack(AMainCharacter* instigatorCharacter, FVector dir)
{
    UE_LOG(LogTemp, Warning, TEXT("ATTACK BASE EXECUTE ATTACK"));
	if (!instigatorCharacter) return;
	owningCharacter = instigatorCharacter;

    baseRotation = dir.Rotation();

	usesLeft--;

	if (usesLeft <= 0) {
		cooldownLeft = cooldown;
		isOnCooldown = true;
	}
}

void UAttackBase::InitializeAttack()
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

    cooldownLeft = 0;
    usesLeft = uses;
    isOnCooldown = false;
}

void UAttackBase::OnBeat() {
	if (isOnCooldown) {
		cooldownLeft--;
	}

	if (cooldownLeft <= 0 && isOnCooldown) {
		cooldownLeft = 0;
		usesLeft = uses;
		isOnCooldown = false;
	}
}

void UAttackBase::LevelUp() {
	if (level == 6) return;

	level++;

	switch (level) {
    case 1:
        damage = damageRow->Level1;
        cooldown = cooldownRow->Level1;
        projectiles = projectileRow->Level1;
        pierce = pierceRow->Level1;
        uses = usesRow->Level1;
        break;
    case 2:
        damage = damageRow->Level2;
        cooldown = cooldownRow->Level2;
        projectiles = projectileRow->Level2;
        pierce = pierceRow->Level2;
        uses = usesRow->Level2;
        break;
    case 3:
        damage = damageRow->Level3;
        cooldown = cooldownRow->Level3;
        projectiles = projectileRow->Level3;
        pierce = pierceRow->Level3;
        uses = usesRow->Level3;
        break;
    case 4:
        damage = damageRow->Level4;
        cooldown = cooldownRow->Level4;
        projectiles = projectileRow->Level4;
        pierce = pierceRow->Level4;
        uses = usesRow->Level4;
        break;
    case 5:
        damage = damageRow->Level5;
        cooldown = cooldownRow->Level5;
        projectiles = projectileRow->Level5;
        pierce = pierceRow->Level5;
        uses = usesRow->Level5;
        break;
    case 6:
        damage = damageRow->Level6;
        cooldown = cooldownRow->Level6;
        projectiles = projectileRow->Level6;
        pierce = pierceRow->Level6;
        uses = usesRow->Level6;
        break;
    }

    	cooldownLeft = 0;
		usesLeft = uses;
		isOnCooldown = false;

	UE_LOG(LogTemp, Warning, TEXT("%s is now level %d"), *weaponName.ToString(), level);
}
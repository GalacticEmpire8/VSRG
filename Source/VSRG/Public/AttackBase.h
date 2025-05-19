// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <VSRGGameMode.h>
#include "WeaponData.h"
#include "AttackBase.generated.h"

class AProjectile;
class AMainCharacter;

/**
 *
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class VSRG_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:
	UAttackBase();

	int32 damage;
	int32 cooldown;
	int32 projectiles;
	int32 pierce;
	int32 uses;
	int32 range;
	
	int rarity;

	FWeaponData* damageRow;
	FWeaponData* cooldownRow;
	FWeaponData* projectileRow;
	FWeaponData* pierceRow;
	FWeaponData* usesRow;
	FWeaponData* rangeRow;

	AMainCharacter* owningCharacter;

	// called when the attack is triggered
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void executeAttack(AMainCharacter* instigatorCharacter);
	
	void initializeAttack();

	void onBeat();

	void levelUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FDataTableRowHandle DamageData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FDataTableRowHandle CooldownData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FDataTableRowHandle ProjectileData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FDataTableRowHandle PierceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FDataTableRowHandle UsesData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FDataTableRowHandle RangeData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> projectileActor;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	int usesLeft;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	bool isOnCooldown;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	int cooldownLeft;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	int level;
};

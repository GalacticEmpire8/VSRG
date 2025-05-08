// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <VSRGGameMode.h>
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
	// called when the attack is triggered
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void executeAttack(AMainCharacter* instigatorCharacter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int projectileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int pierceCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int usesCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackRange;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> projectileActor;
};

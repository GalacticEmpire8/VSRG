// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <MainCharacter.h>
#include <Projectile.h>
#include <WeaponData.h>

void USniperAttack::InitializeAttack()
{
	Super::InitializeAttack();
	critChance = 5;
	critMultiplier = 2;
}

void USniperAttack::ExecuteAttack(AMainCharacter* instigatorCharacter, FVector dir)
{
	Super::ExecuteAttack(instigatorCharacter, dir);

	// Get spawn location and direction
	FVector spawnLocation = owningCharacter->GetActorLocation();
	FVector forwardVector = baseRotation.Vector();
	
	float finalDamage = damage;

	bool isCrit = (FMath::RandRange(1, 100) <= critChance);
	if (isCrit) finalDamage *= critMultiplier;

	// Spawn projectile
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileActor, spawnLocation, baseRotation);
	if (projectile)
	{
		projectile->damage = finalDamage;
		projectile->MaximumPirceCount = pierce;
		projectile->SetActorRotation(baseRotation);
		projectile->projectileComponent->Velocity = forwardVector * projectile->projectileComponent->InitialSpeed;
		projectile->SetLifeSpan(1.f);
		projectile->SetOwner(owningCharacter);
	}
}

void USniperAttack::LevelUp()
{
	Super::LevelUp();

	switch (level) {
	case 3:
		critChance += 5;
	case 5:
		critChance += 5;
	case 6:
		critMultiplier = 1000;
	}
}

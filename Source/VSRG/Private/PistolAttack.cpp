// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <MainCharacter.h>
#include <Projectile.h>
#include <WeaponData.h>

void UPistolAttack::InitializeAttack()
{
	Super::InitializeAttack();

	maxBullets = 0;
}

void UPistolAttack::ExecuteAttack(AMainCharacter* instigatorCharacter, FVector dir)
{
	Super::ExecuteAttack(instigatorCharacter, dir);

	UE_LOG(LogTemp, Warning, TEXT("Pistol Attack"));
	bulletsFired = 0;
	maxBullets = projectiles;
	UWorld* world = instigatorCharacter->GetWorld();
	if (world) {
		world->GetTimerManager().SetTimer(burstTimerHandle, this, &UPistolAttack::FireBurstProjectile, timeBetweenBullets, true);
	}
}

void UPistolAttack::FireBurstProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("FIRING"))

		if (bulletsFired >= maxBullets)
		{
			GetWorld()->GetTimerManager().ClearTimer(burstTimerHandle);
			return;
		}

	// Get spawn location and direction
	FVector spawnLocation = owningCharacter->GetActorLocation();
	FVector forwardVector = baseRotation.Vector();

	// Spawn projectile
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileActor, spawnLocation, baseRotation);
	if (projectile)
	{
		projectile->damage = damage;
		projectile->SetActorRotation(baseRotation);
		projectile->projectileComponent->Velocity = forwardVector * projectile->projectileComponent->InitialSpeed;
		projectile->SetLifeSpan(0.2f);
		projectile->SetOwner(owningCharacter);
	}

	bulletsFired++;
}
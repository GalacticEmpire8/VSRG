// Fill out your copyright notice in the Description page of Project Settings.


#include "SMGAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <MainCharacter.h>
#include <Projectile.h>
#include <WeaponData.h>

void USMGAttack::InitializeAttack()
{
	Super::InitializeAttack();

	maxBullets = 0;
}

void USMGAttack::ExecuteAttack(AMainCharacter* instigatorCharacter, FVector dir)
{
	Super::ExecuteAttack(instigatorCharacter, dir);

	bulletsFired = 0;
	maxBullets = projectiles;
	UWorld* world = instigatorCharacter->GetWorld();
	if (world) {
		world->GetTimerManager().SetTimer(burstTimerHandle, this, &USMGAttack::FireBurstProjectile, timeBetweenBullets, true);
	}
}

void USMGAttack::FireBurstProjectile()
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

	float spreadAngle = FMath::DegreesToRadians(sprayAngle);
	float randomAngle = FMath::FRandRange(-spreadAngle, spreadAngle);
	FQuat randomRotation = FQuat(FVector::UpVector, randomAngle);
	FVector projectileDirection = randomRotation.RotateVector(forwardVector);

	// Spawn projectile
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileActor, spawnLocation, baseRotation);
	if (projectile)
	{
		projectile->damage = damage;
		projectile->SetActorRotation(projectileDirection.Rotation());
		projectile->projectileComponent->Velocity = projectileDirection * projectile->projectileComponent->InitialSpeed;
		projectile->SetLifeSpan(0.2f);
		projectile->SetOwner(owningCharacter);
	}

	bulletsFired++;
}
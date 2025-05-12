// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifleAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <MainCharacter.h>
#include <Projectile.h>
#include <WeaponData.h>



UAssaultRifleAttack::UAssaultRifleAttack()
{
	level = 1;
	maxBullets = 0;
	owningCharacter = nullptr;
}

void UAssaultRifleAttack::initializeAttack()
{
	Super();
}

void UAssaultRifleAttack::executeAttack_Implementation(AMainCharacter* instigatorCharacter)
{
	if (!instigatorCharacter) return;
	owningCharacter = instigatorCharacter;

	UE_LOG(LogTemp, Warning, TEXT("Damage: %d"), damage);

	bulletsFired = 0;
	maxBullets = projectiles;
	UWorld* world = instigatorCharacter->GetWorld();
	if (world) {
		UE_LOG(LogTemp, Warning, TEXT("world exists"));
		world->GetTimerManager().SetTimer(burstTimerHandle, this, &UAssaultRifleAttack::FireBurstProjectile, timeBetweenBullets, true);
	}
	else UE_LOG(LogTemp, Warning, TEXT("world does not exist"));
}

void UAssaultRifleAttack::FireBurstProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("FIRING"))

		if (bulletsFired >= maxBullets)
		{
			UE_LOG(LogTemp, Warning, TEXT("bullets fired: %d, max bullets: %d - returning"), bulletsFired, maxBullets);
			GetWorld()->GetTimerManager().ClearTimer(burstTimerHandle);
			return;
		}

	UE_LOG(LogTemp, Warning, TEXT("bullets fired: %d, max bullets: %d - firing projectiles"), bulletsFired, maxBullets)

	// Get spawn location and direction
	FVector spawnLocation = owningCharacter->GetActorLocation();
	FRotator spawnRotation = owningCharacter->inputDirection.Rotation();
	FVector forwardVector = spawnRotation.Vector();
	float spreadAngle = FMath::DegreesToRadians(sprayAngle);
	float randomAngle = FMath::FRandRange(-spreadAngle, spreadAngle);
	FQuat randomRotation = FQuat(FVector::UpVector, randomAngle);
	FVector projectileDirection = randomRotation.RotateVector(forwardVector);

	// Spawn projectile
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileActor, spawnLocation, spawnRotation);
	if (projectile)
	{
		projectile->damage = damage;
		projectile->SetActorRotation(projectileDirection.Rotation());
		projectile->projectileComponent->Velocity = projectileDirection * projectile->projectileComponent->InitialSpeed;
		projectile->SetLifeSpan(5.f);
		projectile->SetOwner(owningCharacter);
	}

	bulletsFired++;
}


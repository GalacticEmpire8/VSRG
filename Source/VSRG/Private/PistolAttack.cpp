// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <MainCharacter.h>
#include <Projectile.h>
#include <WeaponData.h>

UPistolAttack::UPistolAttack()
{
	level = 1;
	maxBullets = 0;
	owningCharacter = nullptr;
}

void UPistolAttack::initializeAttack()
{
	Super::initializeAttack();
}

void UPistolAttack::executeAttack_Implementation(AMainCharacter* instigatorCharacter)
{
	Super::executeAttack_Implementation(instigatorCharacter);

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
	FRotator spawnRotation = owningCharacter->inputDirection.Rotation();
	FVector forwardVector = spawnRotation.Vector();

	// Spawn projectile
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileActor, spawnLocation, spawnRotation);
	if (projectile)
	{
		projectile->damage = damage;
		projectile->SetActorRotation(spawnRotation);
		projectile->projectileComponent->Velocity = forwardVector * projectile->projectileComponent->InitialSpeed;
		projectile->SetLifeSpan(0.2f);
		projectile->SetOwner(owningCharacter);
	}

	bulletsFired++;
}
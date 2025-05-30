// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <MainCharacter.h>
#include <Projectile.h>
#include <WeaponData.h>

void UShotgunAttack::InitializeAttack()
{
	Super::InitializeAttack();
    coneHalfAngleDegrees = 70.0f;
}

void UShotgunAttack::ExecuteAttack(AMainCharacter* instigatorCharacter, FVector dir)
{
    Super::ExecuteAttack(instigatorCharacter, dir);

    UWorld* world = instigatorCharacter->GetWorld();
    if (!world) return;

    FVector spawnLocation = owningCharacter->GetActorLocation();

    for (int32 i = 0; i < projectiles; ++i)
    {
        // Calculate the yaw offset for this projectile
        float fraction = (projectiles == 1) ? 0.5f : (float)i / (projectiles - 1);
        float yawOffset = FMath::Lerp(-coneHalfAngleDegrees, coneHalfAngleDegrees, fraction);

        // Create the rotation for this projectile
        FRotator shotRotation = baseRotation;
        shotRotation.Yaw += yawOffset;

        // Optionally offset spawn location to avoid self-collision
        FVector shotDirection = shotRotation.Vector();
        FVector spawnOffset = shotDirection * 30.f;
        FVector finalSpawnLocation = spawnLocation + spawnOffset;

        // Spawn the projectile with the correct rotation
        AProjectile* projectile = world->SpawnActor<AProjectile>(projectileActor, finalSpawnLocation, shotRotation);
        if (projectile)
        {
            projectile->damage = damage;
            projectile->SetOwner(owningCharacter);
            projectile->SetActorRotation(shotDirection.Rotation());
            projectile->projectileComponent->Velocity = shotDirection * projectile->projectileComponent->InitialSpeed;
            projectile->SetLifeSpan(0.1f);
        }
    }
}

void UShotgunAttack::LevelUp() {
    Super::LevelUp();

    switch (level) {
    case 2:
        coneHalfAngleDegrees = 60.0f;
    case 4:
        coneHalfAngleDegrees = 40.0f;
    case 6:
        coneHalfAngleDegrees = 20.0f;
    }
}


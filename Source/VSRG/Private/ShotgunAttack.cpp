// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <MainCharacter.h>
#include <Projectile.h>
#include <WeaponData.h>

UShotgunAttack::UShotgunAttack()
{
	level = 1;
	owningCharacter = nullptr;
}

void UShotgunAttack::initializeAttack()
{
	Super::initializeAttack();
}

void UShotgunAttack::executeAttack_Implementation(AMainCharacter* instigatorCharacter)
{
    Super::executeAttack_Implementation(instigatorCharacter);

    UWorld* world = instigatorCharacter->GetWorld();
    if (!world) return;

    FVector spawnLocation = owningCharacter->GetActorLocation();
    FRotator baseRotation = owningCharacter->inputDirection.Rotation();
    FVector forwardVector = baseRotation.Vector();

    float coneHalfAngleDegrees = 30.0f; // Can be a variable or property

    // Uniformly distribute projectiles in the cone
    for (int32 i = 0; i < projectiles; ++i)
    {
        // Calculate the angle for this projectile
        float fraction = (projectiles == 1) ? 0.5f : (float)i / (projectiles - 1);
        float yaw = FMath::Lerp(-coneHalfAngleDegrees, coneHalfAngleDegrees, fraction);

        // Optionally, you can also spread in pitch for a 2D grid, but here's a 1D horizontal spread:
        FRotator shotRotation = baseRotation;
        shotRotation.Yaw += yaw;

        FVector shotDirection = shotRotation.Vector();

        // Spawn projectile
        AProjectile* projectile = world->SpawnActor<AProjectile>(projectileActor, spawnLocation, shotRotation);
        if (projectile)
        {
            projectile->damage = damage;
            projectile->SetActorRotation(shotDirection.Rotation());
            projectile->projectileComponent->Velocity = shotDirection * projectile->projectileComponent->InitialSpeed;
            projectile->SetLifeSpan(0.1f);
            projectile->SetOwner(owningCharacter);
        }
    }
}



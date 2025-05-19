// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <MainCharacter.h>
#include <Projectile.h>
#include <WeaponData.h>

void UShotgunAttack::initializeAttack()
{
	Super::initializeAttack();
    coneHalfAngleDegrees = 30.0f;
}

void UShotgunAttack::executeAttack_Implementation(AMainCharacter* instigatorCharacter)
{
    Super::executeAttack_Implementation(instigatorCharacter);

    UWorld* world = instigatorCharacter->GetWorld();
    if (!world) return;

    FVector spawnLocation = owningCharacter->GetActorLocation();
    FRotator baseRotation = owningCharacter->inputDirection.Rotation();
    FVector forwardVector = baseRotation.Vector();

    // Uniformly distribute projectiles in the cone
    for (int32 i = 0; i < projectiles; ++i)
    {
        // Calculate the angle for this projectile
        float fraction = (projectiles == 1) ? 0.5f : (float)i / (projectiles - 1);
        float yaw = FMath::Lerp(-coneHalfAngleDegrees, coneHalfAngleDegrees, fraction);

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

void UShotgunAttack::levelUp() {
    Super::levelUp();

    switch (level) {
    case 2:
        coneHalfAngleDegrees = 30.0f;
    case 4:
        coneHalfAngleDegrees = 25.0f;
    case 6:
        coneHalfAngleDegrees = 10.0f;
    }
}


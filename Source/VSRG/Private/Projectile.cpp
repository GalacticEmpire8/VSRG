// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Containers/Set.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = projectileMesh;

	projectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	projectileComponent->InitialSpeed = 1000;
	projectileComponent->MaxSpeed = 8500;
	projectileComponent->ProjectileGravityScale = 0.0;

	projectileMesh->SetCollisionProfileName(TEXT("Projectile"));
	projectileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Overlap enemies
	projectileMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore other projectiles
	projectileMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore); // Ignore level geometry
	projectileMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore); // Ignore movable geometry
	projectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	projectileMesh->IgnoreActorWhenMoving(GetOwner(), true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Projectile Spawned"));
	if (GetOwner())
	{
		projectileMesh->IgnoreActorWhenMoving(GetOwner(), true);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("Hit %s"), *OtherActor->GetName());
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner && !OtherActor->IsA(AProjectile::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, damage, MyOwnerInstigator, this, DamageTypeClass);
		if (PirceCount <= MaximumPirceCount)
		{
			PirceCount++;
			//UE_LOG(LogTemp, Display, TEXT("Projectile Destroyed after hitting %d times"), PirceCount);
		}
		else 
		{ 
			Destroy();
			//UE_LOG(LogTemp, Display, TEXT("Projectile hit %d times"), PirceCount);
		}
	}
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                            bool bFromSweep, const FHitResult& SweepResult)
{
    AActor* MyOwner = GetOwner();
    if (MyOwner == nullptr)
    {
        return;
    }

    // Only damage if we haven't already
    if (OtherActor && OtherActor != this && OtherActor != MyOwner && !OtherActor->IsA(AProjectile::StaticClass()))
    {
        if (!DamagedActors.Contains(OtherActor))
        {
            DamagedActors.Add(OtherActor);

            AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
            UClass* DamageTypeClass = UDamageType::StaticClass();

            UGameplayStatics::ApplyDamage(OtherActor, damage, MyOwnerInstigator, this, DamageTypeClass);

            if (PirceCount <= MaximumPirceCount)
            {
                PirceCount++;
            }
            else
            {
                Destroy();
            }
        }
    }
}


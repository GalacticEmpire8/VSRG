// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = projectileMesh;

	projectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	projectileComponent->InitialSpeed = 1000;
	projectileComponent->MaxSpeed = 8500;
	projectileComponent->ProjectileGravityScale = 0.0;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Display, TEXT("Projectile spawned"));
	projectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	/*/if (projectileMesh) {
	//	UE_LOG(LogTemp, Display, TEXT("There is a projectile mesh"));
	}/**/
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
	}
	Destroy();
}
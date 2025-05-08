// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemy.h"
#include "VSRGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

// Sets default values
ABasicEnemy::ABasicEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABasicEnemy::HandleDestruction()
{
	Destroy();
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	VSRGGameMode = Cast<AVSRGGameMode>(GetWorld()->GetAuthGameMode());
	if (VSRGGameMode) {
		//UE_LOG(LogTemp, Warning, TEXT("The BPM is %d"), VSRGGameMode->getBPM());
	}
}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABasicEnemy::OnBeat()
{
	//TODO:Moves on the beat
	//UE_LOG(LogTemp, Warning, TEXT("Move atempted"));
	Move();

}

void ABasicEnemy::Move()
{
	//TODO:Move toward the player
	//Somthing like if(player){
	//get player location
	//if player is further in the x direction than the enemy, move in the x direction vice versa
	//if its equal move both
	//}

	Player = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(this, AMainCharacter::StaticClass()));
	if (Player) {
		FVector PlayerLocation = Player->GetActorLocation();
		FVector EnemyLocation = GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Player location is %s"), *PlayerLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Enemy location is %s"), *EnemyLocation.ToString());
		if ((abs(PlayerLocation.X - EnemyLocation.X) == abs(PlayerLocation.Y - EnemyLocation.Y)))
		{
			if (abs(PlayerLocation.X > EnemyLocation.X) && abs(PlayerLocation.Y > EnemyLocation.Y)) {
				SetActorLocation(FVector(EnemyLocation.X + 100, EnemyLocation.Y + 100, EnemyLocation.Z));
			}
			else if (abs(PlayerLocation.X < EnemyLocation.X) && abs(PlayerLocation.Y < EnemyLocation.Y)) {
				SetActorLocation(FVector(EnemyLocation.X - 100, EnemyLocation.Y - 100, EnemyLocation.Z));
			}
			else if (abs(PlayerLocation.X < EnemyLocation.X) && abs(PlayerLocation.Y > EnemyLocation.Y)) {
				SetActorLocation(FVector(EnemyLocation.X - 100, EnemyLocation.Y + 100, EnemyLocation.Z));
			}
			else if (abs(PlayerLocation.X > EnemyLocation.X) && abs(PlayerLocation.Y > EnemyLocation.Y)) {
				SetActorLocation(FVector(EnemyLocation.X - 100, EnemyLocation.Y + 100, EnemyLocation.Z));
			}
		}
		else if (abs(PlayerLocation.X - EnemyLocation.X) > abs(PlayerLocation.Y - EnemyLocation.Y)) {
			if (PlayerLocation.X > EnemyLocation.X) {
				SetActorLocation(FVector(EnemyLocation.X + 100, EnemyLocation.Y, EnemyLocation.Z));
			}
			else if (PlayerLocation.X < EnemyLocation.X) {
				SetActorLocation(FVector(EnemyLocation.X - 100, EnemyLocation.Y, EnemyLocation.Z));
			}
		}
		else if (abs(PlayerLocation.X - EnemyLocation.X) < abs(PlayerLocation.Y - EnemyLocation.Y))
		{
			if (PlayerLocation.Y > EnemyLocation.Y) {
				SetActorLocation(FVector(EnemyLocation.X, EnemyLocation.Y + 100, EnemyLocation.Z));
			}
			else if (PlayerLocation.Y < EnemyLocation.Y) {
				SetActorLocation(FVector(EnemyLocation.X, EnemyLocation.Y - 100, EnemyLocation.Z));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Player and enemy are in the same location"));
		}
	}
}

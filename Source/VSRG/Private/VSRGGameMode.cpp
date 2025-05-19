// Fill out your copyright notice in the Description page of Project Settings.


#include "VSRGGameMode.h"
#include "BasicEnemy.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
//#include "Engine/World.h"

AVSRGGameMode::AVSRGGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.SetTickFunctionEnable(true);

    audioTrack = CreateDefaultSubobject<USoundBase>(TEXT("Music Track"));
}

void AVSRGGameMode::ActorDied(AActor* DeadActor)
{
    if (DeadActor == Player)
    {
        Player->HandleDestruction();
    }
    else if (ABasicEnemy* DestroyedEnemy = Cast<ABasicEnemy>(DeadActor))
    {
        DestroyedEnemy->HandleDestruction();
        Player->AddXP(DestroyedEnemy->xpOnDeath);
    }
}

int AVSRGGameMode::getBPM()
{
    return BPM;
}

void AVSRGGameMode::BeginPlay()
{
    Super::BeginPlay();
    class APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
    Player = Cast<AMainCharacter>(playerController->GetPawn());
    UGameplayStatics::PlaySound2D(GetWorld(), audioTrack, 1, 1, 0, NULL, this, true);

    UE_LOG(LogTemp, Warning, TEXT("Player is %s"), *Player->GetName());
    UE_LOG(LogTemp, Warning, TEXT("Play has begun"));
}

void AVSRGGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //UE_LOG(LogTemp, Warning, TEXT("DeltaTime is %f"), DeltaTime);
    //UE_LOG(LogTemp, Warning, TEXT("Get world delta seconds is %f"), UGameplayStatics::GetWorldDeltaSeconds(this));
    TimeBetweenLastBeat = TimeBetweenLastBeat + UGameplayStatics::GetWorldDeltaSeconds(this);
    //UE_LOG(LogTemp, Warning, TEXT("Time between last beat is %f"), TimeBetweenLastBeat);
    if (SecondsPerMinute / BPM <= TimeBetweenLastBeat)
    {
        OnBeat();
        TimeBetweenLastBeat = 0;
    }
}

void AVSRGGameMode::OnBeat()
{
    BasicEnemy = Cast<ABasicEnemy>(UGameplayStatics::GetActorOfClass(this, ABasicEnemy::StaticClass()));
    UE_LOG(LogTemp, Warning, TEXT("BEAT"));

    if (BasicEnemy) { BasicEnemy->OnBeat(); }
    if (Player) { Player->OnBeat(); }
    if (CurrentEnemies < MaxEnemies) { 
        //AActor* 
    //    ABasicEnemy* SpawnActor(BasicEnemy);
    //    CurrentEnemies++;
    //    UE_LOG(LogTemp, Display, TEXT("Enemy spawned"));
    //    UE_LOG(LogTemp, Display, TEXT("Current ammount of enemies is %d"),CurrentEnemies);
        SpawnEnemy();
    }
}
//AActor* SpawnActor
//(
  //  UClass* InClass,
    //FVector const* Location,
    //FRotator const* Rotation,
    //const FActorSpawnParameters& SpawnParameters
//)

void AVSRGGameMode::BeatTimer(float DeltaTime)
{

}

bool AVSRGGameMode::IsOnBeat()
{
    // Calculate the time for a single beat
    float BeatDuration = SecondsPerMinute / BPM;
    UE_LOG(LogTemp, Warning, TEXT("If %f >= %f OR %f <= %f, then return true"), TimeBetweenLastBeat, BeatDuration - beatTolerance, TimeBetweenLastBeat, 0.0 + beatTolerance);

    // Check if the current time is within the tolerance window around the beat
    if (TimeBetweenLastBeat >= (BeatDuration - beatTolerance) || TimeBetweenLastBeat <= (0.0 + beatTolerance))
    {
        return true;
    }

    return false;
}

//void AVSRGGameMode::SpawnEnemy()
//{

//}
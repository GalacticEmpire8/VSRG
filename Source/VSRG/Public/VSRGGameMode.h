// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VSRGGameMode.generated.h"

/**
 *
 */
UCLASS()
class VSRG_API AVSRGGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	class ABasicEnemy* BasicEnemy;
	class AMainCharacter* Player;

	// Beats per Minute
	float BPM = 80; // I changed the BPM to the song's. Hope this doesn't break anything - Gabriel
	const float beatTolerance = 0.35;

	// Seconds in a minute
	float SecondsPerMinute = 60;

	//Time between beats
	float TimeBetweenLastBeat;

	

	//Max ammount of enemies
	int MaxEnemies = 5;

	//When beat should be called
	void BeatTimer(float DeltaTime);

	//TODO:Function that handles the start of the game
	//Function that prompts things that act on the beat
	void OnBeat();

	//Funtion that handles spawning enemies
	//void SpawnEnemy();


	//Spawn information
	//FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);
	//FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	//FActorSpawnParameters SpawnInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Audio Track") class USoundBase* audioTrack;

public:
	AVSRGGameMode();

	//Current ammount of enemies
	int CurrentEnemies = 0;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnEnemy();

	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
	FVector ActorLocation();

	void ActorDied(AActor* DeadActor);

	//gets the BPM
	int getBPM();
	bool IsOnBeat();

	// Begin play
	virtual void BeginPlay() override;

	// Tick function
	virtual void Tick(float DeltaTime) override;
};

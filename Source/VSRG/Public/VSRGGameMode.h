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
	int BPM = 60;
	const float beatTolerance = 0.3;

	// Seconds in a minute
	int SecondsPerMinute = 60;

	//Time between beats
	float TimeBetweenLastBeat;

	//When beat should be called
	void BeatTimer(float DeltaTime);

	//TODO:Function that handles the start of the game
	//TODO:Function that prompts things that act on the beat
	void OnBeat();
	//TODO:Fucntion that handles actor death

	UPROPERTY(EditDefaultsOnly, Category = "Audio Track") class USoundBase* audioTrack;

public:
	AVSRGGameMode();

	void ActorDied(AActor* DeadActor);

	//gets the BPM
	int getBPM();
	bool IsOnBeat();

	// Begin play
	virtual void BeginPlay() override;

	// Tick function
	virtual void Tick(float DeltaTime) override;
};

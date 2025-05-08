// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "VSRGGameMode.h"
#include "InputAction.h"
#include "MainCharacter.generated.h"


class UAttackBase;
class AProjectile;

/**
 *
 */
UCLASS()
class VSRG_API AMainCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	void HandleDestruction();

protected:
	virtual void BeginPlay() override;

private:
	class AVSRGGameMode* VSRGGameMode;

	bool shouldTakeStep;
	bool onBeat;
	bool isAttacking;
	bool hasMovedThisBeat = false;

	//input functions
	void Move(FVector axisValue);
	void OnMoveKeyPressed();
	void OnMoveKeyReleased();
	void EnhancedInputMove(const FInputActionValue& Value);

	void UseAttack(FName slot);
	void OnAttackKeyPressed();
	void OnAttackKeyReleased();

public:
	FVector inputDirection;

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void OnBeat();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputMappingContext* inputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* inputToMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* inputToAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* QAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* WAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* EAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* RAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TMap<FName, UAttackBase*> attackSlots;

};

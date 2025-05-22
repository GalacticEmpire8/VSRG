 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "VSRGGameMode.h"
#include "InputAction.h"
#include "WeaponSelectionWidget.h"
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
	bool isAttacking;
	bool onBeat;
	bool moveKeyDown;


	//input functions
	void Move(FVector axisValue);
	void OnMoveKeyPressed();
	void OnMoveKeyReleased();
	void EnhancedInputMove(const FInputActionValue& Value);

	void UseAttack(FName slot);
	void OnAttackKeyPressed();
	void OnAttackKeyReleased();

	void CycleWeaponCooldowns();
	void LevelUp();

public:
	FVector inputDirection;

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void OnBeat();
	void AddXP(float amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "On Beat")
	bool hasMovedThisBeat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "On Beat") // Checking if input is on beat
	bool inputOnBeat;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade System")
	UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade System")
	UWeaponSelectionWidget* WeaponSelectionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UWeaponSelectionWidget> WeaponSelectionWidgetClass;

	UFUNCTION(BlueprintCallable)
	void GrantWeapon(TSubclassOf<UAttackBase> WeaponClass);

	UFUNCTION()
	void Debug_AddXP();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
	float xpToNextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
	float xp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
	int level;
};

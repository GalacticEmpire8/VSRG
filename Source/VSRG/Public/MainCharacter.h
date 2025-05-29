 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "VSRGGameMode.h"
#include "InputAction.h"
#include "WeaponSelectionWidget.h"
#include "WeaponDataRow.h"
#include "HealthComponent.h"
#include "MainCharacter.generated.h"


class UAttackBase;
class UPassiveBase;
class AProjectile;

struct FItemOption
{
	FItemDataRow* DataRow;
	int32 Rarity;
	bool bIsOwned;
};

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

	void UseAttack(const FInputActionValue& Value);
	void OnAttackKeyPressed();
	void OnAttackKeyReleased();

	void CycleWeaponCooldowns();
	void EquipWeapon(int32 slot);
	void LevelUp();

public:
	UAttackBase* equippedWeapon;
	TArray<UPassiveBase*> passiveItems;

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
	UInputAction* slot1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* slot2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* slot3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* slot4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TMap<int32, UAttackBase*> attackSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade System")
	UDataTable* itemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade System")
	UWeaponSelectionWidget* weaponSelectionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UWeaponSelectionWidget> weaponSelectionWidgetClass;

	UFUNCTION(BlueprintCallable)
	void GrantWeapon(TSubclassOf<UItem> WeaponClass);

	void GrantPassive(TSubclassOf<UItem> PassiveClass);

	UFUNCTION()
	void Debug_AddXP();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* healthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
	float xpToNextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
	float xp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
	int level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float defense;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Projectile.h"
#include "AttackBase.h"
#include "EnhancedInputSubsystems.h"

AMainCharacter::AMainCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	xp = 0;
	xpToNextLevel = 5;
	level = 1;
	moveKeyDown = false;
	hasMovedThisBeat = false;
}

void AMainCharacter::HandleDestruction()
{
	Destroy();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	VSRGGameMode = Cast<AVSRGGameMode>(GetWorld()->GetAuthGameMode());

     // Fix for the error: Replace the usage of GetKeys() with a manual iteration to collect keys.  
     TArray<int32> keys;  
     for (const TPair<int32, UAttackBase*>& Pair : attackSlots)  
     {  
        keys.Add(Pair.Key);  
     }  

     // Now you can use the Keys array as needed.  
     for (const int32& key : keys)  
     {  
		 if (UAttackBase** attackPtr = attackSlots.Find(key))
		 {
			 if (*attackPtr)
			 {
				 equippedWeapon = *attackPtr;
				 equippedWeapon->InitializeAttack();
			 }
		 }
     }
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);

	//Enhanced Input
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InInputComponent);

	if (enhancedInputComponent)
	{
		enhancedInputComponent->BindAction(inputToMove, ETriggerEvent::Started, this, &AMainCharacter::OnMoveKeyPressed);
		enhancedInputComponent->BindAction(inputToMove, ETriggerEvent::Triggered, this, &AMainCharacter::EnhancedInputMove);
		enhancedInputComponent->BindAction(inputToMove, ETriggerEvent::Completed, this, &AMainCharacter::OnMoveKeyReleased);

		enhancedInputComponent->BindAction(inputToAttack, ETriggerEvent::Started, this, &AMainCharacter::OnAttackKeyPressed);
		enhancedInputComponent->BindAction(inputToAttack, ETriggerEvent::Triggered, this, &AMainCharacter::UseAttack);
		enhancedInputComponent->BindAction(inputToAttack, ETriggerEvent::Completed, this, &AMainCharacter::OnAttackKeyReleased);

		enhancedInputComponent->BindAction(slot1, ETriggerEvent::Triggered, this, &AMainCharacter::EquipWeapon, int32(1));
		enhancedInputComponent->BindAction(slot2, ETriggerEvent::Triggered, this, &AMainCharacter::EquipWeapon, int32(2));
		enhancedInputComponent->BindAction(slot3, ETriggerEvent::Triggered, this, &AMainCharacter::EquipWeapon, int32(3));
		enhancedInputComponent->BindAction(slot4, ETriggerEvent::Triggered, this, &AMainCharacter::EquipWeapon, int32(4));

		UE_LOG(LogTemp, Warning, TEXT("Movement Setup"));
	}

	// add input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		// get local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			// add input context
			Subsystem->AddMappingContext(inputMappingContext, 0);
		}
	}

	InInputComponent->BindKey(EKeys::P, IE_Pressed, this, &AMainCharacter::Debug_AddXP);
}

void AMainCharacter::Move(FVector axisValue)
{
	FVector direction, startLoc, endLoc;
	float stepSize = 32;

	direction = axisValue *= stepSize;

	startLoc = GetActorLocation();
	endLoc = startLoc + direction;

	FVector NewLoc = FMath::Lerp(startLoc, endLoc, 1.0f);

	SetActorLocation(NewLoc);
	shouldTakeStep = false;
}

void AMainCharacter::EnhancedInputMove(const FInputActionValue& Value)
{
	FVector2D moveValue = Value.Get<FVector2D>();

	if (hasMovedThisBeat) return;
	if (isAttacking) return;

	if (VSRGGameMode->IsOnBeat()) {
		if (shouldTakeStep) {
			CycleWeaponCooldowns();
			if (moveValue.X > 0.05f || moveValue.X < -0.05f) {
				FVector moveDirection = FVector(moveValue.X, 0.0f, 0.0f);
				Move(moveDirection);
			}

			if (moveValue.Y > 0.05f || moveValue.Y < -0.05f) {
				FVector moveDirection = FVector(0.0f, moveValue.Y, 0.0f);
				Move(moveDirection);
			}
		}

		hasMovedThisBeat = true;
	}

	// Check if the input is on the beat
	if (VSRGGameMode && VSRGGameMode->IsOnBeat())
	{
		UE_LOG(LogTemp, Warning, TEXT("Input is on the beat!"));
		inputOnBeat = true;
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input is off the beat."));
		inputOnBeat = false;
	}
}

void AMainCharacter::OnMoveKeyPressed()
{
	shouldTakeStep = true;
	moveKeyDown = true;
}

void AMainCharacter::OnMoveKeyReleased()
{
	shouldTakeStep = false;
	moveKeyDown = false;
}

void AMainCharacter::OnBeat()
{
	hasMovedThisBeat = false;
}

void AMainCharacter::UseAttack(const FInputActionValue& Value)
{
	if (hasMovedThisBeat) return;

	FVector2D dirValue = Value.Get<FVector2D>();

	if (VSRGGameMode->IsOnBeat()) {
		if (equippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Equppied weapon"));
			if (!equippedWeapon->isOnCooldown) { 
				CycleWeaponCooldowns();

				if (dirValue.X > 0.05f || dirValue.X < -0.05f) {
					FVector inputDirection = FVector(dirValue.X, 0.0f, 0.0f);
					equippedWeapon->ExecuteAttack(this, inputDirection);
				}

				if (dirValue.Y > 0.05f || dirValue.Y < -0.05f) {
					FVector inputDirection = FVector(0.0f, dirValue.Y, 0.0f);
					equippedWeapon->ExecuteAttack(this, inputDirection);
				}

				UE_LOG(LogTemp, Warning, TEXT("Executing attack"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("Attack is on cooldown!"));
		}

		hasMovedThisBeat = true;
		isAttacking = false;
	}
	else UE_LOG(LogTemp, Warning, TEXT("Attack off beat"));
}

void AMainCharacter::EquipWeapon(int32 slot)
{
	if (UAttackBase** attackPtr = attackSlots.Find(slot))
	{
		if (*attackPtr)
		{
			equippedWeapon = *attackPtr;
		}
	}
}

void AMainCharacter::OnAttackKeyPressed()
{
	isAttacking = true;
	shouldTakeStep = false;
}

void AMainCharacter::OnAttackKeyReleased()
{
	isAttacking = false;
}

void AMainCharacter::CycleWeaponCooldowns() {
	TArray<int32> keys;
	for (const TPair<int32, UAttackBase*>& Pair : attackSlots)
	{
		keys.Add(Pair.Key);
	}

	// Now you can use the Keys array as needed.  
	for (const int32& key : keys)
	{
		if (UAttackBase** attackPtr = attackSlots.Find(key))
		{
			if (*attackPtr)
			{
				(*attackPtr)->OnBeat();
			}
		}
	}
}

void AMainCharacter::AddXP(float amount) {
	xp += amount;
	UE_LOG(LogTemp, Warning, TEXT("Debug: Added 10 XP"));

	if (xp >= xpToNextLevel) {
		UE_LOG(LogTemp, Warning, TEXT("Level Up"));
		LevelUp();
	}
}

void AMainCharacter::LevelUp() {
	level++;
	xp = 0;

	if (level <= 10) xpToNextLevel += 10;
	else if (level <= 20) xpToNextLevel += 13;
	else xpToNextLevel += 16;

	if (!weaponDataTable) return;

	TArray<FWeaponDataRow*> AllWeapons;
	static const FString ContextString(TEXT("Weapon Selection"));
	weaponDataTable->GetAllRows<FWeaponDataRow>(ContextString, AllWeapons);

	int32 NumChoices = 3;
	if (AllWeapons.Num() < NumChoices) return;

	// Build owned weapon set and option pool, and track level 6 weapons
	TMap<UClass*, int32> OwnedWeaponLevels;
	for (const TPair<int32, UAttackBase*>& Pair : attackSlots) {
		if (Pair.Value) {
			OwnedWeaponLevels.Add(Pair.Value->GetClass(), Pair.Value->level);
		}
	}

	// Check if all 4 slots are filled (i.e., player owns 4 weapons)
	bool bAllSlotsFilled = true;
	static const TArray<int32> SlotOrder = { 1, 2, 3, 4 };
	for (const int32& Slot : SlotOrder)
	{
		UAttackBase** WeaponPtr = attackSlots.Find(Slot);
		if (!WeaponPtr || *WeaponPtr == nullptr)
		{
			bAllSlotsFilled = false;
			break;
		}
	}

	TArray<FWeaponOption> Pool;
	TArray<FWeaponOption> OwnedOptions;
	for (FWeaponDataRow* Row : AllWeapons) {
		if (!Row || !Row->WeaponClass) continue;
		int32* OwnedLevel = OwnedWeaponLevels.Find(Row->WeaponClass);
		bool bIsOwned = OwnedLevel != nullptr;
		bool bIsMaxed = bIsOwned && *OwnedLevel >= 6;
		if (bIsMaxed) continue; // Skip maxed weapons

		// If all slots are filled, only offer owned weapons
		if (bAllSlotsFilled && !bIsOwned) continue;

		FWeaponOption Option{ Row, Row->rarity, bIsOwned };
		Pool.Add(Option);
		if (bIsOwned) OwnedOptions.Add(Option);
	}

	// Calculate ownedChance
	float totalLuck = 1.0f; // Replace with your actual luck value
	int32 x = (level % 2 == 0) ? 2 : 1;
	float ownedChance = 1 + 0.3f * x - 1 / totalLuck;

	TArray<FWeaponOption> Selected;
	TSet<FWeaponDataRow*> AlreadySelected;

	// Try to select up to 2 owned items
	for (int32 ownedRoll = 0; ownedRoll < 2 && Selected.Num() < NumChoices; ++ownedRoll) {
		if (OwnedOptions.Num() == 0) break;
		if (FMath::FRand() < ownedChance) {
			// Pick a random owned item not already selected
			TArray<FWeaponOption> AvailableOwned;
			for (const FWeaponOption& Option : OwnedOptions) {
				if (!AlreadySelected.Contains(Option.DataRow))
					AvailableOwned.Add(Option);
			}
			if (AvailableOwned.Num() > 0) {
				int32 idx = FMath::RandRange(0, AvailableOwned.Num() - 1);
				Selected.Add(AvailableOwned[idx]);
				AlreadySelected.Add(AvailableOwned[idx].DataRow);
			}
		}
	}

	// Fill remaining slots with weighted random selection from pool (excluding already selected)
	while (Selected.Num() < NumChoices) {
		// Remove already selected from pool
		TArray<FWeaponOption> AvailablePool;
		int32 totalWeight = 0;
		for (const FWeaponOption& Option : Pool) {
			if (!AlreadySelected.Contains(Option.DataRow)) {
				AvailablePool.Add(Option);
				totalWeight += Option.Rarity;
			}
		}
		if (AvailablePool.Num() == 0 || totalWeight == 0) break;

		int32 randWeight = FMath::RandRange(0, totalWeight - 1);
		int32 runningWeight = 0;
		for (const FWeaponOption& Option : AvailablePool) {
			runningWeight += Option.Rarity;
			if (randWeight < runningWeight) {
				Selected.Add(Option);
				AlreadySelected.Add(Option.DataRow);
				break;
			}
		}
	}

	// Prepare the final weapon class array for the widget
	TArray<TSubclassOf<UAttackBase>> WeaponClasses;
	for (const FWeaponOption& Option : Selected) {
		if (Option.DataRow && Option.DataRow->WeaponClass) {
			WeaponClasses.Add(Option.DataRow->WeaponClass);
		}
	}

	if (weaponSelectionWidgetClass) {
		weaponSelectionWidget = CreateWidget<UWeaponSelectionWidget>(GetWorld(), weaponSelectionWidgetClass);
		if (weaponSelectionWidget) {
			weaponSelectionWidget->InitWeaponOptions(WeaponClasses); // Only contains available options
			weaponSelectionWidget->AddToViewport();

			if (APlayerController* PC = Cast<APlayerController>(GetController())) {
				PC->SetPause(true);
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(weaponSelectionWidget->TakeWidget());
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
		}
	}
}

void AMainCharacter::GrantWeapon(TSubclassOf<UAttackBase> WeaponClass)
{
	if (!WeaponClass) return;

	// Check if the weapon already exists in any slot
	for (auto& Pair : attackSlots)
	{
		if (Pair.Value && Pair.Value->GetClass() == WeaponClass)
		{
			Pair.Value->LevelUp();
			return;
		}
	}

	// If not found, add to the first available slot
	UAttackBase* NewWeapon = NewObject<UAttackBase>(this, WeaponClass);
	if (!NewWeapon) return;

	static const TArray<int32> SlotOrder = { 1, 2, 3, 4 };
	for (const int32& Slot : SlotOrder)
	{
		if (!attackSlots.Contains(Slot) || attackSlots[Slot] == nullptr)
		{
			attackSlots.Add(Slot, NewWeapon);
			NewWeapon->InitializeAttack();
			return;
		}
	}
}

void AMainCharacter::Debug_AddXP()
{
	AddXP(100.0f); // Adds 10 XP, adjust as needed
}
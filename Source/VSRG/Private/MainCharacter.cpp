// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Projectile.h"
#include "AttackBase.h"
#include "WeaponDataRow.h"
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
				 equippedWeapon->initializeAttack();
			 }
		 }
     }
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	VSRGGameMode = Cast<AVSRGGameMode>(GetWorld()->GetAuthGameMode());
	//UE_LOG(LogTemp, Warning, TEXT("The BPM is %d"), VSRGGameMode->getBPM());
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
		enhancedInputComponent->BindAction(inputToAttack, ETriggerEvent::Triggered, this, &AMainCharacter::OnAttackKeyPressed);
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

void AMainCharacter::EnhancedInputMove(const FInputActionValue& Value)
{
	FVector2D moveValue = Value.Get<FVector2D>();
	inputDirection = FVector(moveValue.X, moveValue.Y, 0.0f);

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

void AMainCharacter::OnBeat()
{
	hasMovedThisBeat = false;
}

void AMainCharacter::UseAttack(const FInputActionValue& Value)
{
	if (hasMovedThisBeat) return;

	FVector2D dirValue = Value.Get<FVector2D>();
	inputDirection = FVector(dirValue.X, dirValue.Y, 0.0f);

	if (VSRGGameMode->IsOnBeat()) {
		if (equippedWeapon)
		{
			if (!equippedWeapon->isOnCooldown) { 
				CycleWeaponCooldowns();
				equippedWeapon->ExecuteAttack(this, inputDirection); 
			}
			else UE_LOG(LogTemp, Warning, TEXT("Attack is on cooldown!"));
		}

		hasMovedThisBeat = true;
		isAttacking = false;
	}
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
				(*attackPtr)->onBeat();
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

	// Build a set of weapon classes the player owns at level 6
	TSet<UClass*> MaxedWeaponClasses;
	for (const TPair<int32, UAttackBase*>& Pair : attackSlots) {
		if (Pair.Value && Pair.Value->level >= 6) {
			MaxedWeaponClasses.Add(Pair.Value->GetClass());
		}
	}

	// Filter out weapons the player owns at level 6
	TArray<FWeaponDataRow*> AvailableWeapons;
	for (FWeaponDataRow* WeaponRow : AllWeapons) {
		if (WeaponRow && WeaponRow->WeaponClass) {
			if (!MaxedWeaponClasses.Contains(WeaponRow->WeaponClass)) {
				AvailableWeapons.Add(WeaponRow);
			}
		}
	}

	if (AvailableWeapons.Num() < NumChoices) return;

	// Shuffle
	for (int32 i = AvailableWeapons.Num() - 1; i > 0; --i) {
		int32 j = FMath::RandRange(0, i);
		AvailableWeapons.Swap(i, j);
	}

	TArray<TSubclassOf<UAttackBase>> WeaponClasses;
	for (int32 i = 0; i < NumChoices; ++i) {
		if (AvailableWeapons[i]->WeaponClass) {
			WeaponClasses.Add(AvailableWeapons[i]->WeaponClass);
		}
	}

	if (weaponSelectionWidgetClass) {
		weaponSelectionWidget = CreateWidget<UWeaponSelectionWidget>(GetWorld(), weaponSelectionWidgetClass);
		if (weaponSelectionWidget) {
			weaponSelectionWidget->InitWeaponOptions(WeaponClasses);
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
			Pair.Value->levelUp();
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
			NewWeapon->initializeAttack();
			return;
		}
	}
}

void AMainCharacter::Debug_AddXP()
{
	AddXP(10.0f); // Adds 10 XP, adjust as needed
}
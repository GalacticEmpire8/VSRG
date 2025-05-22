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
     TArray<FName> keys;  
     for (const TPair<FName, UAttackBase*>& Pair : attackSlots)  
     {  
        keys.Add(Pair.Key);  
     }  

     // Now you can use the Keys array as needed.  
     for (const FName& key : keys)  
     {  
		 if (UAttackBase** attackPtr = attackSlots.Find(key))
		 {
			 if (*attackPtr)
			 {
				 (*attackPtr)->initializeAttack();
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

		enhancedInputComponent->BindAction(QAttack, ETriggerEvent::Started, this, &AMainCharacter::OnAttackKeyPressed);
		enhancedInputComponent->BindAction(QAttack, ETriggerEvent::Triggered, this, &AMainCharacter::UseAttack, FName("Q"));
		enhancedInputComponent->BindAction(QAttack, ETriggerEvent::Completed, this, &AMainCharacter::OnAttackKeyReleased);

		enhancedInputComponent->BindAction(WAttack, ETriggerEvent::Started, this, &AMainCharacter::OnAttackKeyPressed);
		enhancedInputComponent->BindAction(WAttack, ETriggerEvent::Triggered, this, &AMainCharacter::UseAttack, FName("W"));
		enhancedInputComponent->BindAction(WAttack, ETriggerEvent::Completed, this, &AMainCharacter::OnAttackKeyReleased);

		enhancedInputComponent->BindAction(EAttack, ETriggerEvent::Started, this, &AMainCharacter::OnAttackKeyPressed);
		enhancedInputComponent->BindAction(EAttack, ETriggerEvent::Triggered, this, &AMainCharacter::UseAttack, FName("E"));
		enhancedInputComponent->BindAction(EAttack, ETriggerEvent::Completed, this, &AMainCharacter::OnAttackKeyReleased);

		enhancedInputComponent->BindAction(RAttack, ETriggerEvent::Started, this, &AMainCharacter::OnAttackKeyPressed);
		enhancedInputComponent->BindAction(RAttack, ETriggerEvent::Triggered, this, &AMainCharacter::UseAttack, FName("R"));
		enhancedInputComponent->BindAction(RAttack, ETriggerEvent::Completed, this, &AMainCharacter::OnAttackKeyReleased);
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

void AMainCharacter::UseAttack(FName slot)
{
	if (hasMovedThisBeat) return;

	if (VSRGGameMode->IsOnBeat()) {
		if (!isAttacking) {
			UE_LOG(LogTemp, Warning, TEXT("not using attack"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("using attack"));

		// Check if the attack slot is valid
		if (UAttackBase** attackPtr = attackSlots.Find(slot))
		{
			if (*attackPtr)
			{
				if (!(*attackPtr)->isOnCooldown) { 
					CycleWeaponCooldowns();
					(*attackPtr)->executeAttack(this); 
				}
				else UE_LOG(LogTemp, Warning, TEXT("Attack is on cooldown!"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("Cant find slot %s"), *slot.ToString());
		}

		hasMovedThisBeat = true;
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
	TArray<FName> keys;
	for (const TPair<FName, UAttackBase*>& Pair : attackSlots)
	{
		keys.Add(Pair.Key);
	}

	// Now you can use the Keys array as needed.  
	for (const FName& key : keys)
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

	if (!WeaponDataTable) return;

	TArray<FWeaponDataRow*> AllWeapons;
	static const FString ContextString(TEXT("Weapon Selection"));
	WeaponDataTable->GetAllRows<FWeaponDataRow>(ContextString, AllWeapons);

	int32 NumChoices = 3;
	if (AllWeapons.Num() < NumChoices) return;

	// Shuffle
	for (int32 i = AllWeapons.Num() - 1; i > 0; --i)
	{
		int32 j = FMath::RandRange(0, i);
		AllWeapons.Swap(i, j);
	}

	// Select first N
	TArray<TSubclassOf<UAttackBase>> WeaponClasses;
	for (int32 i = 0; i < NumChoices; ++i)
	{
		if (AllWeapons[i]->WeaponClass)
		{
			WeaponClasses.Add(AllWeapons[i]->WeaponClass);
		}
	}

	if (WeaponSelectionWidgetClass)
	{
		WeaponSelectionWidget = CreateWidget<UWeaponSelectionWidget>(GetWorld(), WeaponSelectionWidgetClass);
		if (WeaponSelectionWidget)
		{
			WeaponSelectionWidget->InitWeaponOptions(WeaponClasses);
			WeaponSelectionWidget->AddToViewport();

			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				PC->SetPause(true);
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(WeaponSelectionWidget->TakeWidget());
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
			UE_LOG(LogTemp, Log, TEXT("Leveled up weapon in slot %s"), *Pair.Key.ToString());
			return;
		}
	}

	// If not found, add to the first available slot
	UAttackBase* NewWeapon = NewObject<UAttackBase>(this, WeaponClass);
	if (!NewWeapon) return;

	static const TArray<FName> SlotOrder = { FName("Q"), FName("W"), FName("E"), FName("R") };
	for (const FName& Slot : SlotOrder)
	{
		if (!attackSlots.Contains(Slot) || attackSlots[Slot] == nullptr)
		{
			attackSlots.Add(Slot, NewWeapon);
			NewWeapon->initializeAttack();
			UE_LOG(LogTemp, Log, TEXT("Granted weapon to slot %s"), *Slot.ToString());
			return;
		}
	}
}

void AMainCharacter::Debug_AddXP()
{
	AddXP(10.0f); // Adds 10 XP, adjust as needed
}
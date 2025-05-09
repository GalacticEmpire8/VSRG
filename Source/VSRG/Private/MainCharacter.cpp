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

}

void AMainCharacter::HandleDestruction()
{
	Destroy();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

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
}

void AMainCharacter::OnMoveKeyReleased()
{
	shouldTakeStep = false;
}

void AMainCharacter::EnhancedInputMove(const FInputActionValue& Value)
{
	FVector2D moveValue = Value.Get<FVector2D>();
	inputDirection = FVector(moveValue.X, moveValue.Y, 0.0f);

	if (hasMovedThisBeat) return;
	if (isAttacking) return;

	if (VSRGGameMode->IsOnBeat()) {
		if (shouldTakeStep) {
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
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input is off the beat."));
	}
}

void AMainCharacter::OnBeat()
{
	hasMovedThisBeat = false;
}

void AMainCharacter::UseAttack(FName slot)
{
	if (hasMovedThisBeat) return;
	if (!VSRGGameMode->IsOnBeat()) return;

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
			(*attackPtr)->executeAttack(this);
		}
		else UE_LOG(LogTemp, Warning, TEXT("Cant find slot %s"), *slot.ToString());
	}

	hasMovedThisBeat = true;
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "JamCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AJamCharacterBase::AJamCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Autoposess player on play
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create and attach camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	camera->SetRelativeLocation(FVector(0, 0, 40));
	camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AJamCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	currentJamPlayerState = EJamPlayerState::IDLE;
}

// Called every frame
void AJamCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("State: %i"), currentJamPlayerState));

	switch (currentJamPlayerState)
	{
	case EJamPlayerState::IDLE:
		// Transition to Moving
		if (verticalMovementInputValue != 0 ||
			horizontalMovementInputValue != 0)
		{
			currentJamPlayerState = EJamPlayerState::MOVING;
		}
		break;
	case EJamPlayerState::MOVING:
		ApplyMovement();

		// Transition to Idle
		if (verticalMovementInputValue == 0 &&
			horizontalMovementInputValue == 0)
		{
			currentJamPlayerState = EJamPlayerState::IDLE;
		}
		break;
	case EJamPlayerState::SPRINTING:
		ApplyMovement();
		break;
	case EJamPlayerState::CROUCHING:
		ApplyMovement();
		break;
	case EJamPlayerState::FALLING:
		break;
	}
}

// Called to bind functionality to input
void AJamCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Move X", this, &AJamCharacterBase::InputMoveHorizontal);
	InputComponent->BindAxis("Move Y", this, &AJamCharacterBase::InputMoveVertical);
	InputComponent->BindAxis("Aim X", this, &AJamCharacterBase::InputAimHorizontal);
	InputComponent->BindAxis("Aim Y", this, &AJamCharacterBase::InputAimVertical);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AJamCharacterBase::InputSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AJamCharacterBase::InputUnSprint);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

}

void AJamCharacterBase::InputMoveHorizontal(float value)
{
	horizontalMovementInputValue = value;
}

void AJamCharacterBase::InputMoveVertical(float value)
{
	verticalMovementInputValue = value;
}

void AJamCharacterBase::InputAimHorizontal(float value)
{
	if (!value) return;
	AddControllerYawInput(value);
}

void AJamCharacterBase::InputAimVertical(float value)
{
	if (!value) return;
	if (InvertY) value *= -1;
	AddControllerPitchInput(value);
}

void AJamCharacterBase::InputSprint()
{
	if (verticalMovementInputValue > 0 && !GetCharacterMovement()->IsCrouching())
		GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed * SprintSpeedMultiplier;
}

void AJamCharacterBase::InputUnSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
}

void AJamCharacterBase::ApplyMovement()
{
	if (horizontalMovementInputValue != 0)
	{
		AddMovementInput(GetActorRightVector(), horizontalMovementInputValue);
	}

	if (verticalMovementInputValue != 0)
	{
		AddMovementInput(GetActorForwardVector(), verticalMovementInputValue);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "JamCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AJamCharacterBase::AJamCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create and attach camera boom
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	cameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	cameraBoom->SetRelativeLocation(FVector(0, 0, 40));
	cameraBoom->TargetArmLength = 0;
	cameraBoom->bUsePawnControlRotation = true;
	cameraBoom->bEnableCameraLag = true;

	// Create and attach camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->AttachToComponent(cameraBoom, FAttachmentTransformRules::KeepWorldTransform);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called when the game starts or when spawned
void AJamCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	currentJamPlayerState = EJamPlayerState::IDLE;
	baseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
}

// Called every frame
void AJamCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("State: %i"), currentJamPlayerState));

	// JamPlayerState FSM
	switch (currentJamPlayerState)
	{
	case EJamPlayerState::IDLE:
		// Transition to Crouching
		if (bCrouchInputValue)
		{
			Crouch();
			currentJamPlayerState = EJamPlayerState::CROUCHING;
		}
		// Transition to Moving
		if (verticalMovementInputValue != 0 ||
			horizontalMovementInputValue != 0)
		{
			currentJamPlayerState = EJamPlayerState::MOVING;
		}
		// Transition to Falling
		if (!GetCharacterMovement()->IsMovingOnGround())
		{
			currentJamPlayerState = EJamPlayerState::FALLING;
		}
		// Transition to Falling via jumping
		if (bJumpInputValue == true)
		{
			currentJamPlayerState = EJamPlayerState::FALLING;
			if (!bWasJumping)
				Jump();
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
		// Transition to Crouching
		if (bCrouchInputValue)
		{
			Crouch();
			currentJamPlayerState = EJamPlayerState::CROUCHING;
		}
		// Transition to Falling
		if (!GetCharacterMovement()->IsMovingOnGround())
		{
			currentJamPlayerState = EJamPlayerState::FALLING;
		}
		// Transition to Falling via jumping
		if (bJumpInputValue == true)
		{
			currentJamPlayerState = EJamPlayerState::FALLING;
			if (!bWasJumping)
				Jump();
		}
		if (bSprintInputValue && verticalMovementInputValue > 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed * SprintSpeedMultiplier;
			currentJamPlayerState = EJamPlayerState::SPRINTING;
		}
		break;

	case EJamPlayerState::SPRINTING:
		ApplyMovement();

		// Transition to Idle
		if (verticalMovementInputValue == 0 &&
			horizontalMovementInputValue == 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
			currentJamPlayerState = EJamPlayerState::IDLE;
		}
		// Transition to Moving
		if (!bSprintInputValue)
		{
			GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
			currentJamPlayerState = EJamPlayerState::MOVING;
		}
		// Transition to Crouching
		if (bCrouchInputValue)
		{
			GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
			Crouch();
			currentJamPlayerState = EJamPlayerState::CROUCHING;
		}
		// Transition to Falling
		if (!GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
			currentJamPlayerState = EJamPlayerState::FALLING;
		}
		// Transition to Falling via jumping
		if (bJumpInputValue == true)
		{
			currentJamPlayerState = EJamPlayerState::FALLING;
			if (!bWasJumping)
				Jump();
		}
		break;

	case EJamPlayerState::CROUCHING:		// TODO: Implement smooth camera
		ApplyMovement();

		// Transition to Falling
		if (!GetCharacterMovement()->IsMovingOnGround())
		{
			currentJamPlayerState = EJamPlayerState::FALLING;
		}
		// Transition to IDLE
		if (!bCrouchInputValue)
		{
			UnCrouch();
			currentJamPlayerState = EJamPlayerState::IDLE;
		}
		break;

	case EJamPlayerState::FALLING:
		// Transition to Idle
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			// Ensure sprint gets reset
			GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
			currentJamPlayerState = EJamPlayerState::IDLE;
			if (bWasJumping && !bJumpInputValue)
				StopJumping();
		}
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

	InputComponent->BindAction("Jump", IE_Pressed, this, &AJamCharacterBase::InputJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AJamCharacterBase::InputUnJump);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &AJamCharacterBase::InputCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &AJamCharacterBase::InputUnCrouch);
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
	bSprintInputValue = true;
	if (verticalMovementInputValue > 0 && !GetCharacterMovement()->IsCrouching())
		GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed * SprintSpeedMultiplier;
}

void AJamCharacterBase::InputUnSprint()
{
	bSprintInputValue = false;
	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
}

void AJamCharacterBase::InputJump()
{
	bJumpInputValue = true;
}

void AJamCharacterBase::InputUnJump()
{
	bJumpInputValue = false;
}

void AJamCharacterBase::InputCrouch()
{
	bCrouchInputValue = true;
}

void AJamCharacterBase::InputUnCrouch()
{
	bCrouchInputValue = false;
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


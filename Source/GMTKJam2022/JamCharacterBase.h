// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JamCharacterBase.generated.h"


UENUM(BlueprintType)
enum class EJamPlayerState : uint8
{
	IDLE		UMETA(DisplayName = "Idle"),
	MOVING		UMETA(DisplayName = "Moving"),
	SPRINTING	UMETA(DisplayName = "Sprinting"),
	CROUCHING	UMETA(DisplayName = "Crouching"),
	FALLING		UMETA(DisplayName = "Falling")
};

UCLASS()
class GMTKJAM2022_API AJamCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AJamCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InputMoveHorizontal(float value);
	void InputMoveVertical(float value);
	void InputAimHorizontal(float value);
	void InputAimVertical(float value);
	void InputSprint();
	void InputUnSprint();
	void InputJump();
	void InputUnJump();
	void InputCrouch();
	void InputUnCrouch();
	void InputInteract();

	void ApplyMovement();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacter|Camera")
	float MouseSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacter|Camera")
	bool InvertY = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacter|Movement")
	float SprintSpeedMultiplier = 2.0f;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class  USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, Category = "JamCharacter")
	class UJamCharacterInteractPoint* jamCharacterInteractPoint;

	UPROPERTY(VisibleAnywhere, Category = "JamCharacter")
	class UPhysicsHandleComponent* physicsHandle;

	UPROPERTY(BlueprintReadOnly, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
	bool bJumpInputValue = false;

	UPROPERTY(BlueprintReadOnly, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
	bool bSprintInputValue = false;

	UPROPERTY(BlueprintReadOnly, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
	bool bCrouchInputValue = false;

	UPROPERTY(BlueprintReadOnly, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
	float verticalMovementInputValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
	float horizontalMovementInputValue = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
	float capsuleStartHeight = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
	float baseWalkSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
	EJamPlayerState currentJamPlayerState;

	float cameraInitialHeight;
};

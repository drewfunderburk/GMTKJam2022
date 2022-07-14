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
	// Sets default values for this character's properties
	AJamCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InputMoveHorizontal(float value);
	void InputMoveVertical(float value);
	void InputAimHorizontal(float value);
	void InputAimVertical(float value);
	void InputSprint();
	void InputUnSprint();

	void ApplyMovement();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacter|Camera")
		float MouseSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacter|Camera")
		bool InvertY = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacter|Movement")
		float SprintSpeedMultiplier = 2.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacter", meta = (AllowPrivateAccess = "true"))
		float interactRange = 250.0f;

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

	FCollisionShape initialCapsuleCollisionShape;
};

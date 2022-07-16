// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop.h"
#include "DataAssetLevels.h"
#include "Dice.generated.h"

UCLASS()
class GMTKJAM2022_API ADice : public AProp
{
	GENERATED_BODY()
	
public:
	ADice();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Dice")
	void OnRandomizeLevel();
	void Throw();

private:
	bool IsMovingOrRotating() const;
	void RandomizeLevel();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dice", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dice", meta = (AllowPrivateAccess = "true"))
	float stoppedThreshold = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Dice", meta = (AllowPrivateAccess = "true"))
	bool bHasHitSomething = false;

	UPROPERTY(BlueprintReadWrite, Category = "Dice", meta = (AllowPrivateAccess = "true"))
	bool bHasBeenThrown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dice", meta = (AllowPrivateAccess = "true"))
	UDataAssetLevels* Levels;

};

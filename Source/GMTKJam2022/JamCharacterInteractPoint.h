// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "JamCharacterInteractPoint.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GMTKJAM2022_API UJamCharacterInteractPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	UJamCharacterInteractPoint();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "JamCharacterInteractPoint")
	void SphereTraceFromPointToPoint(FVector start, FHitResult& hit);
	void Interact(FVector sphereCastStartPosition, class UPhysicsHandleComponent* physicsHandle);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacterInteractPoint")
	float SphereTraceRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JamCharacterInteractPoint")
	float GrabLerpSpeed = 1.0f;

private:

};

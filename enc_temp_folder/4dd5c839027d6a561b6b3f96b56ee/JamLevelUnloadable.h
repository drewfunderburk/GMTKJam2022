// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JamLevelUnloadable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJamLevelUnloadable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GMTKJAM2022_API IJamLevelUnloadable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnJamLevelUnload();
};

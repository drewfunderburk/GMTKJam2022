// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssetLevels.generated.h"

/**
 * 
 */
UCLASS()
class GMTKJAM2022_API UDataAssetLevels : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levels")
	TArray<TSoftObjectPtr<UWorld>> Levels;
};

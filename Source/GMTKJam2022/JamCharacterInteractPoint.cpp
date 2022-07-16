// Fill out your copyright notice in the Description page of Project Settings.


#include "JamCharacterInteractPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interactable.h"
#include "Prop.h"

// Sets default values for this component's properties
UJamCharacterInteractPoint::UJamCharacterInteractPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJamCharacterInteractPoint::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UJamCharacterInteractPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (grabbedObject != nullptr)
	{
		FVector newLocation = FMath::Lerp(grabbedObject->GetActorLocation(), GetComponentLocation(), GrabLerpSpeed);
		grabbedObject->SetActorLocation(newLocation);
	}
}

void UJamCharacterInteractPoint::SphereTraceFromActorToPoint(FHitResult& hit)
{
	FVector start = GetOwner()->GetActorLocation();
	FVector end = GetComponentLocation();
	TArray<AActor*> actorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		start,
		end,
		SphereTraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		actorsToIgnore,
		EDrawDebugTrace::ForDuration,
		hit,
		true,
		FLinearColor::Blue,
		FLinearColor::Green,
		3.0f
	);
}

void UJamCharacterInteractPoint::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("JamCharacterInteractPoint: Interact")));
	if (grabbedObject == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("JamCharacterInteractPoint: GrabbedObject was null")));

		FHitResult hit;
		SphereTraceFromActorToPoint(hit);
		AProp* prop = Cast<AProp>(hit.GetActor());

		if (prop == nullptr) return;
		else if (prop->bCanBePickedUp)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("JamCharacterInteractPoint: Grabbed object")));

			grabbedObject = prop;
		}

		if (prop->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_OnInteract(hit.GetActor(), GetOwner());
		}
	}
	else
	{
		grabbedObject = nullptr;
	}
}

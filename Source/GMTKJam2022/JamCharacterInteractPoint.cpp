// Fill out your copyright notice in the Description page of Project Settings.


#include "JamCharacterInteractPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interactable.h"
#include "Prop.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


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
		EDrawDebugTrace::None,
		hit,
		true,
		FLinearColor::Blue,
		FLinearColor::Green,
		3.0f
	);
}

void UJamCharacterInteractPoint::Interact(UPhysicsHandleComponent* physicsHandle)
{
	if (physicsHandle->GrabbedComponent == nullptr)
	{

		FHitResult hit;
		SphereTraceFromActorToPoint(hit);
		AProp* prop = Cast<AProp>(hit.GetActor());

		if (prop == nullptr) return;
		else if (prop->bCanBePickedUp)
		{
			physicsHandle->GrabComponentAtLocationWithRotation(hit.GetComponent(), NAME_None, GetComponentLocation(), prop->GetActorRotation());
		}

		if (prop->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_OnInteract(hit.GetActor(), GetOwner());
		}
	}
	else
	{
		physicsHandle->ReleaseComponent();
	}
}

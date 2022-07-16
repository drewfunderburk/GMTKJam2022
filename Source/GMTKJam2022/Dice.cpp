// Fill out your copyright notice in the Description page of Project Settings.


#include "Dice.h"

ADice::ADice()
{
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = staticMesh;

	staticMesh->SetSimulatePhysics(true);
}

void ADice::BeginPlay()
{
	Super::BeginPlay();

}

void ADice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHasBeenThrown && bHasHitSomething && !IsMovingOrRotating())
	{
		bHasBeenThrown = false;
		bHasHitSomething = false;
		RandomizeLevel();
	}
}

void ADice::Throw()
{
	bHasBeenThrown = true;
}

bool ADice::IsMovingOrRotating() const
{
	float velocity = staticMesh->GetPhysicsAngularVelocityInRadians().SquaredLength();
	float angularVelocity = staticMesh->GetComponentVelocity().SquaredLength();

	return 
		velocity > stoppedThreshold &&
		angularVelocity > stoppedThreshold;
}

void ADice::RandomizeLevel()
{
	OnRandomizeLevel();
}

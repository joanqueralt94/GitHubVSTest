// Fill out your copyright notice in the Description page of Project Settings.

#include "AttractableActorComponent.h"
#include "kismet/gameplaystatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UAttractableActorComponent::UAttractableActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttractableActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttractableActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_AttractingActor != nullptr)
	{
		Attraction(GetOwner(), m_AttractingActor, DeltaTime, AttractSpeed);
	}

}

void UAttractableActorComponent::StartAttraction(AActor* AttractingActor)
{
	SetIsAttracting(AttractingActor != nullptr);
	m_AttractingActor = AttractingActor;
}

void UAttractableActorComponent::Attraction(AActor* OwnerActor, AActor* AttractingActor, float DeltaTime, float AttractionSpeed)
{
	check(OwnerActor != nullptr && AttractingActor != nullptr);
	
	//Moving Position (No physics approach)
	/*
	FVector AttractVector = AttractingActor->GetActorLocation() - OwnerActor->GetActorLocation();
	AttractVector = AttractVector.GetSafeNormal();
	const FVector NewVector = OwnerActor->GetActorLocation() + AttractVector * AttractionSpeed * DeltaTime;
	OwnerActor->SetActorLocation(NewVector);*/


	//Moving Position with Forces and Physics
	FVector AttractVector = AttractingActor->GetActorLocation() - OwnerActor->GetActorLocation();
	AttractVector = AttractVector.GetSafeNormal();

	// Calculate the force vector to apply.
	FVector AttractionForceVector = AttractVector * AttractionSpeed;

	// Assuming the root component of the owner actor is a UPrimitiveComponent.
	USceneComponent* OwnerRootComponent = OwnerActor->GetRootComponent();

	if (OwnerRootComponent)
	{
		// Check if the root component is a UPrimitiveComponent.
		UPrimitiveComponent* PrimitiveRootComponent = Cast<UPrimitiveComponent>(OwnerRootComponent);

		if (PrimitiveRootComponent)
		{
			// Apply the force to the root component.
			PrimitiveRootComponent->AddForce(AttractionForceVector, NAME_None, true); // true indicates to use the acceleration change.
		}
		else
		{
			// Handle the case where the root component is not a UPrimitiveComponent.
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "OwnerRootComponent is null for the attracted actor");
		}
	}

}
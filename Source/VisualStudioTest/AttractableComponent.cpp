// Fill out your copyright notice in the Description page of Project Settings.


#include "AttractableComponent.h"

// Sets default values for this component's properties
UAttractableComponent::UAttractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAttractableComponent::Attraction(AActor* BaseActor, AActor* AttractingActor, float DeltaTime)
{
	if (bIsAttracting)
	{
		FVector AttractVector = AttractingActor->GetActorLocation() - BaseActor->GetActorLocation();
		AttractVector = AttractVector.GetSafeNormal();
		FVector NewVector = BaseActor->GetActorLocation() + AttractVector * AttractSpeed * DeltaTime;
		BaseActor->SetActorLocation(NewVector);
	}
}


// Called when the game starts
void UAttractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
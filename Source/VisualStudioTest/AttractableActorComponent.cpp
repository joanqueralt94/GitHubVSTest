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
	FVector AttractVector = AttractingActor->GetActorLocation() - OwnerActor->GetActorLocation();
	AttractVector = AttractVector.GetSafeNormal();
	const FVector NewVector = OwnerActor->GetActorLocation() + AttractVector * AttractionSpeed * DeltaTime;
	OwnerActor->SetActorLocation(NewVector);


		
}
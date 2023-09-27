// Fill out your copyright notice in the Description page of Project Settings.

#include "AttractionActorComponent.h"
#include "kismet/gameplaystatics.h"
#include "AttractableActorComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UAttractionActorComponent::UAttractionActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAttractionActorComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UAttractionActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetbIsAttractInputPressed())
	{
		AngleDistanceCalculation();
	}

}

void UAttractionActorComponent::Raycast()
{
	FVector Start = GetOwner()->GetActorLocation();
	const FVector ForwardVector = GetOwner()->GetActorForwardVector();

	const FVector End = Start + (ForwardVector * RaycastDistance);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	const UWorld* MyWorld = GetWorld();
	check(MyWorld != nullptr);

	static constexpr bool bPersistentLines = false;
	static constexpr float LifeTime = 2.f;
	static constexpr uint8 DepthPriority = 0;
	static constexpr float Thickness = 2.f;

	DrawDebugLine(MyWorld, Start, End, FColor::Yellow, bPersistentLines, LifeTime, DepthPriority, Thickness);

	static constexpr ECollisionChannel TraceChannel = ECC_Visibility;

	TArray<FHitResult> OutHitArray;

	MyWorld->LineTraceMultiByChannel(OutHitArray, Start, End, TraceChannel, CollisionParams);

	for (FHitResult& HitResult : OutHitArray)
	{
		AActor* ActorHit = HitResult.GetActor();
		if (ActorHit != nullptr && ActorHit->ActorHasTag(TEXT("Attractable")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *ActorHit->GetName());

			if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(ActorHit))
			{
				m_AttractedActors.AddUnique(ActorHit);
				AttractableComponent->StartAttraction(GetOwner());
			}
		}
	}
}

void UAttractionActorComponent::RaycastTimer()
{
	ResetAttractedActors(GetAttractedActors());

	FVector Start = GetOwner()->GetActorLocation();
	const FVector ForwardVector = GetOwner()->GetActorForwardVector();

	const FVector End = Start + (ForwardVector * RaycastDistance);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	const UWorld* MyWorld = GetWorld();
	check(MyWorld != nullptr);

	static constexpr bool bPersistentLines = false;
	static constexpr float LifeTime = 2.f;
	static constexpr uint8 DepthPriority = 0;
	static constexpr float Thickness = 2.f;

	DrawDebugLine(MyWorld, Start, End, FColor::Yellow, bPersistentLines, LifeTime, DepthPriority, Thickness);

	static constexpr ECollisionChannel TraceChannel = ECC_Visibility;

	TArray<FHitResult> OutHitArray;

	MyWorld->LineTraceMultiByChannel(OutHitArray, Start, End, TraceChannel, CollisionParams);

	for (FHitResult& HitResult : OutHitArray)
	{
		AActor* ActorHit = HitResult.GetActor();
		if (ActorHit != nullptr && ActorHit->ActorHasTag(TEXT("Attractable")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *ActorHit->GetName());

			if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(ActorHit))
			{
				m_AttractedActors.AddUnique(ActorHit);
				AttractableComponent->StartAttraction(GetOwner());
			}
		}
	}
}

void UAttractionActorComponent::StartAttracting()
{
	switch (GetAttractionMode())
	{
	default:
	case EAttractionMode::Invalid:
		break;
	case EAttractionMode::ViaAngle:

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Function Attract Angle System");
		SetbIsAttractInputPressed(true);
		Raycast();

		break;
	case EAttractionMode::ViaTimer:

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Function Attract Timer System");
		SetbIsAttractInputPressed(true);
		GetWorld()->GetTimerManager().SetTimer(RaycastTimerHandle, this, &UAttractionActorComponent::RaycastTimer, RaycastInterval, true);

		break;
	}
}

void UAttractionActorComponent::StopAttracting()
{
	switch (GetAttractionMode())
	{
	default:
	case EAttractionMode::Invalid:
		break;
	case EAttractionMode::ViaAngle:

		SetbIsAttractInputPressed(false);

		for (AActor* Actor : m_AttractedActors)
		{
			if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(Actor))
			{
				AttractableComponent->StartAttraction(nullptr);
			}
		}

		m_AttractedActors.Reset();

		break;
	case EAttractionMode::ViaTimer:

		SetbIsAttractInputPressed(false);
		GetWorld()->GetTimerManager().ClearTimer(RaycastTimerHandle);
		ResetAttractedActors(GetAttractedActors());

		break;
	}
}

void UAttractionActorComponent::AngleDistanceCalculation()
{
	// Get the character's forward vector
	FVector CharacterForward = GetOwner()->GetActorForwardVector();

	// Create a list of attracted actors to remove
	TArray<AActor*> ActorsToRemove;

	// Iterate through the attracted actors
	for (AActor* AttractedActor : m_AttractedActors)
	{
		if (AttractedActor)
		{
			// Calculate the vector pointing from the character to the attracted actor
			FVector ToActor = AttractedActor->GetActorLocation() - GetOwner()->GetActorLocation();
			ToActor.Normalize();

			// Calculate the dot product between the character's forward vector and the vector to the actor
			const float DotProduct = FVector::DotProduct(CharacterForward, ToActor);

			// Calculate the angle in degrees
			const float AngleInDegrees = FMath::Acos(DotProduct) * (180.0f / PI);

			// If the angle is greater than the threshold, add the actor to the removal list
			if (AngleInDegrees > ms_MaxAttractingAngle)
			{
				ActorsToRemove.Add(AttractedActor);
			}
		}
	}

	// Remove actors that are no longer attracted
	for (AActor* ActorToRemove : ActorsToRemove)
	{
		if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(ActorToRemove))
		{
			AttractableComponent->StartAttraction(nullptr);
		}

		// Remove the actor from the attracted list
		m_AttractedActors.Remove(ActorToRemove);
	}
}

UAttractableActorComponent* UAttractionActorComponent::GetAttractableActorComponent(AActor* Actor) const
{
	if (Actor != nullptr)
	{
		UActorComponent* ActorComponent = Actor->GetComponentByClass(UAttractableActorComponent::StaticClass());

		return Cast<UAttractableActorComponent>(ActorComponent);
	}
	return nullptr;
}

void UAttractionActorComponent::ResetAttractedActors(TArray<AActor*>& AttractedActors)
{
	for (AActor* Actor : AttractedActors)
	{
		if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(Actor))
		{
			AttractableComponent->StartAttraction(nullptr);
		}
	}

	AttractedActors.Reset();
}

void UAttractionActorComponent::ToggleAttractionMode()
{
	if (GetAttractionMode() == EAttractionMode::ViaAngle)
	{
		SetAttractionMode(EAttractionMode::ViaTimer);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "Toggled to Timer Mode");
	}
	else
	{
		SetAttractionMode(EAttractionMode::ViaAngle);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "Toggled to Angle Mode");
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "AttractionActorComponent.h"
#include "kismet/gameplaystatics.h"
#include "AttractableActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

	m_ThirdPersonCharacter = Cast<AVisualStudioTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}


// Called every frame
void UAttractionActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_bIsAttractInputPressed)
	{
		AngleDistanceCalculation();
	}

}

void UAttractionActorComponent::Raycast()
{
	
	switch (CurrentAttractionMode)
	{
	case EAttractionMode::ViaAngle:


		break;
	case EAttractionMode::ViaTimer:

		ResetAttractedActors(m_ThirdPersonCharacter->GetAttractedActors());

		break;
	}

	FVector Start = m_ThirdPersonCharacter->GetFollowCamera()->GetComponentLocation();
	const FVector ForwardVector = m_ThirdPersonCharacter->GetFollowCamera()->GetForwardVector();

	Start = Start + (ForwardVector * m_ThirdPersonCharacter->GetCameraBoom()->TargetArmLength);

	const FVector End = Start + (ForwardVector * m_ThirdPersonCharacter->RaycastDistance);
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
				m_ThirdPersonCharacter->m_AttractedActors.AddUnique(ActorHit);
				AttractableComponent->StartAttraction(m_ThirdPersonCharacter);
			}
		}
	}

}

void UAttractionActorComponent::StartAttracting()
{
	switch (CurrentAttractionMode)
	{
	case EAttractionMode::ViaAngle:

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Function Attract Angle System");
		m_bIsAttractInputPressed = true;
		Raycast();

		break;
	case EAttractionMode::ViaTimer:

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Function Attract Timer System");

		m_bIsAttractInputPressed = true;

		GetWorld()->GetTimerManager().SetTimer(RaycastTimerHandle, this, &UAttractionActorComponent::Raycast, RaycastInterval, true);

		break;
	}
}

void UAttractionActorComponent::StopAttracting()
{
	switch (CurrentAttractionMode)
	{
	case EAttractionMode::ViaAngle:

		m_bIsAttractInputPressed = false;

		for (AActor* Actor : m_ThirdPersonCharacter->m_AttractedActors)
		{
			if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(Actor))
			{
				AttractableComponent->StartAttraction(nullptr);
			}
		}

		m_ThirdPersonCharacter->m_AttractedActors.Reset();

		break;
	case EAttractionMode::ViaTimer:

		m_bIsAttractInputPressed = false;

		GetWorld()->GetTimerManager().ClearTimer(RaycastTimerHandle);

		ResetAttractedActors(m_ThirdPersonCharacter->GetAttractedActors());

		break;
	}
}

void UAttractionActorComponent::AngleDistanceCalculation()
{
	// Get the character's forward vector
	FVector CharacterForward = m_ThirdPersonCharacter->GetActorForwardVector();

	// Create a list of attracted actors to remove
	TArray<AActor*> ActorsToRemove;

	// Iterate through the attracted actors
	for (AActor* AttractedActor : m_ThirdPersonCharacter->m_AttractedActors)
	{
		if (AttractedActor)
		{
			// Calculate the vector pointing from the character to the attracted actor
			FVector ToActor = AttractedActor->GetActorLocation() - m_ThirdPersonCharacter->GetActorLocation();
			ToActor.Normalize();

			// Calculate the dot product between the character's forward vector and the vector to the actor
			const float DotProduct = FVector::DotProduct(CharacterForward, ToActor);

			// Calculate the angle in degrees
			const float AngleInDegrees = FMath::Acos(DotProduct) * (180.0f / PI);

			// If the angle is greater than the threshold, add the actor to the removal list
			if (AngleInDegrees > m_ThirdPersonCharacter->ms_MaxAttractingAngle)
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
		m_ThirdPersonCharacter->m_AttractedActors.Remove(ActorToRemove);
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
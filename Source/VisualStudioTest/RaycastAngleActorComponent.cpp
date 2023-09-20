// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastAngleActorComponent.h"
#include "Engine/Engine.h"
#include "kismet/gameplaystatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
URaycastAngleActorComponent::URaycastAngleActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URaycastAngleActorComponent::BeginPlay()
{
	Super::BeginPlay();

	ThirdPersonCharacter = Cast<AVisualStudioTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}


// Called every frame
void URaycastAngleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsAttractInputPressed)
	{
		// Get the character's forward vector
		FVector CharacterForward = ThirdPersonCharacter->GetActorForwardVector();

		// Create a list of attracted actors to remove
		TArray<AActor*> ActorsToRemove;

		// Iterate through the attracted actors
		for (AActor* AttractedActor : ThirdPersonCharacter->m_AttractedActors)
		{
			if (AttractedActor)
			{
				// Calculate the vector pointing from the character to the attracted actor
				FVector ToActor = AttractedActor->GetActorLocation() - ThirdPersonCharacter->GetActorLocation();
				ToActor.Normalize();

				// Calculate the dot product between the character's forward vector and the vector to the actor
				const float DotProduct = FVector::DotProduct(CharacterForward, ToActor);

				// Calculate the angle in degrees
				const float AngleInDegrees = FMath::Acos(DotProduct) * (180.0f / PI);

				// If the angle is greater than the threshold, add the actor to the removal list
				if (AngleInDegrees > ThirdPersonCharacter->ms_MaxAttractingAngle)
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
				AttractableComponent->StartAttracting(nullptr);
			}

			// Remove the actor from the attracted list
			ThirdPersonCharacter->m_AttractedActors.Remove(ActorToRemove);
		}
	}
}

void URaycastAngleActorComponent::Raycast()
{
	
	FVector Start = ThirdPersonCharacter->GetFollowCamera()->GetComponentLocation();
	const FVector ForwardVector = ThirdPersonCharacter->GetFollowCamera()->GetForwardVector();

	Start = Start + (ForwardVector * ThirdPersonCharacter->GetCameraBoom()->TargetArmLength);

	const FVector End = Start + (ForwardVector * ThirdPersonCharacter->RaycastDistance);
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
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "Actor Hit: " + ActorHit->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *ActorHit->GetName());

			if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(ActorHit))
			{
				ThirdPersonCharacter->m_AttractedActors.AddUnique(ActorHit);
				AttractableComponent->StartAttracting(ThirdPersonCharacter);
			}
		}
	}
}

void URaycastAngleActorComponent::StartAttracting()
{
	bIsAttractInputPressed = true;
	Raycast();
}

void URaycastAngleActorComponent::StopAttracting()
{
	bIsAttractInputPressed = false;

	for (AActor* Actor : ThirdPersonCharacter->m_AttractedActors)
	{
		if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(Actor))
		{
			AttractableComponent->StartAttracting(nullptr);
		}
	}

	ThirdPersonCharacter->m_AttractedActors.Reset();
}

UAttractableActorComponent* URaycastAngleActorComponent::GetAttractableActorComponent(AActor* Actor) const
{
	if (Actor != nullptr)
	{
		UActorComponent* ActorComponent = Actor->GetComponentByClass(UAttractableActorComponent::StaticClass());

		return Cast<UAttractableActorComponent>(ActorComponent);
	}
	return nullptr;
}

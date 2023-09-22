// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastTimerActorComponent.h"
#include "AttractableActorComponent.h"
#include "kismet/gameplaystatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"


// Sets default values for this component's properties
URaycastTimerActorComponent::URaycastTimerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URaycastTimerActorComponent::BeginPlay()
{
	Super::BeginPlay();

	m_ThirdPersonCharacter = Cast<AVisualStudioTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// ...
	
}


// Called every frame
void URaycastTimerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (bIsAttractInputPressed)
	{
	}*/
}

void URaycastTimerActorComponent::Raycast()
{
	ResetAttractedActors(m_ThirdPersonCharacter->GetAttractedActors());

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

void URaycastTimerActorComponent::StartAttracting()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Function Attract Timer System");

	bIsAttractInputPressed = true;

	GetWorld()->GetTimerManager().SetTimer(RaycastTimerHandle, this, &URaycastTimerActorComponent::Raycast, RaycastInterval, true);
}

void URaycastTimerActorComponent::StopAttracting()
{
	bIsAttractInputPressed = false;

	GetWorld()->GetTimerManager().ClearTimer(RaycastTimerHandle);

	ResetAttractedActors(m_ThirdPersonCharacter->GetAttractedActors());
}

UAttractableActorComponent* URaycastTimerActorComponent::GetAttractableActorComponent(AActor* Actor) const
{
	if (Actor != nullptr)
	{
		UActorComponent* ActorComponent = Actor->GetComponentByClass(UAttractableActorComponent::StaticClass());

		return Cast<UAttractableActorComponent>(ActorComponent);
	}
	return nullptr;
}

void URaycastTimerActorComponent::ResetAttractedActors(TArray<AActor*>& AttractedActors)
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

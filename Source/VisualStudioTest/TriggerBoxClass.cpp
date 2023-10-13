// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxClass.h"
#include "DrawDebugHelpers.h"
#include "kismet/gameplaystatics.h"


ATriggerBoxClass::ATriggerBoxClass()
{
	OnActorBeginOverlap.AddDynamic(this, &ATriggerBoxClass::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerBoxClass::OnOverlapEnd);
}

void ATriggerBoxClass::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
	ThirdPersonCharacter = Cast<AVisualStudioTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

void ATriggerBoxClass::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor == ThirdPersonCharacter))
	{
		ThirdPersonCharacter->SpawnObject();
	}
}

void ATriggerBoxClass::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "Overlap Ended with Trigger Box");
		FString ActorMessage = FString::Printf(TEXT("Overlapped with %s"), *OtherActor->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, ActorMessage);
	}
}

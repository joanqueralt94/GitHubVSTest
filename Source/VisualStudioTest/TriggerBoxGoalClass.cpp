// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxGoalClass.h"
#include "DrawDebugHelpers.h"
#include "kismet/gameplaystatics.h"

ATriggerBoxGoalClass::ATriggerBoxGoalClass()
{
	OnActorBeginOverlap.AddDynamic(this, &ATriggerBoxGoalClass::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerBoxGoalClass::OnOverlapEnd);
}

void ATriggerBoxGoalClass::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);
	MeshActor = GetWorld()->SpawnActor<AMeshActor>(AMeshActor::StaticClass());
	ThirdPersonCharacter = Cast<AVisualStudioTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

void ATriggerBoxGoalClass::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(AMeshActor::StaticClass()))
	{
		MeshActor = Cast<AMeshActor>(OtherActor);
		if (MeshActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "GOAL!");
			OtherActor->Destroy();
			ThirdPersonCharacter->m_GoalScore++;
		}
	}
}

void ATriggerBoxGoalClass::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	
}

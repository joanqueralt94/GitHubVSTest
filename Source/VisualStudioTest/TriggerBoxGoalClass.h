// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MeshActor.h"
#include "VisualStudioTestCharacter.h"
#include "TriggerBoxGoalClass.generated.h"


UCLASS()
class VISUALSTUDIOTEST_API ATriggerBoxGoalClass : public ATriggerBox
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	AMeshActor* MeshActor;

	AVisualStudioTestCharacter* ThirdPersonCharacter;


	ATriggerBoxGoalClass();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	
};

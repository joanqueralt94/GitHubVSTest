// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "VisualStudioTestCharacter.h"
#include "TriggerBoxClass.generated.h"

/**
 * 
 */
UCLASS()
class VISUALSTUDIOTEST_API ATriggerBoxClass : public ATriggerBox
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	AVisualStudioTestCharacter* ThirdPersonCharacter;

	ATriggerBoxClass();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
	
};

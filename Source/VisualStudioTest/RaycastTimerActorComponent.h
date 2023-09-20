// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VisualStudioTestCharacter.h"
#include "GameFramework/Character.h"
#include "RaycastTimerActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VISUALSTUDIOTEST_API URaycastTimerActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URaycastTimerActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	AVisualStudioTestCharacter* ThirdPersonCharacter;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Raycast();

	void StartAttracting();

	void StopAttracting();

	bool bIsAttractInputPressed = false;

	class UAttractableActorComponent* GetAttractableActorComponent(AActor* Actor) const;

};

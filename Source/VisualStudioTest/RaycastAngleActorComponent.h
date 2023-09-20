// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttractableActorComponent.h"
#include "VisualStudioTestCharacter.h"
#include "GameFramework/Character.h"
#include "RaycastAngleActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VISUALSTUDIOTEST_API URaycastAngleActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URaycastAngleActorComponent();

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

	void AngleDistanceCalculation();

	bool bIsAttractInputPressed = false;

	class UAttractableActorComponent* GetAttractableActorComponent(AActor* Actor) const;

};

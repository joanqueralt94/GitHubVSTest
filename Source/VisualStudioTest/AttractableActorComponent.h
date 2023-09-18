// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttractableActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VISUALSTUDIOTEST_API UAttractableActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttractableActorComponent();

	UPROPERTY(EditAnywhere, Category = "Attraction")
	float AttractSpeed = 200.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetIsAttracting() const { return bIsAttracting;}
	void SetIsAttracting(bool bValue) { bIsAttracting = bValue;	}
	
	void StartAttracting(AActor* AttractingActor);

private:
	bool bIsAttracting = false;

	UPROPERTY()
	AActor* m_AttractingActor = nullptr;

	void Attraction(AActor* OwnerActor, AActor* AttractingActor, float DeltaTime, float AttractionSpeed);

		
};

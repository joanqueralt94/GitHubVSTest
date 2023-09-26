// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "AttractionActorComponent.generated.h"

class AVisualStudioTestCharacter;

UENUM(BlueprintType)
enum class EAttractionMode : uint8
{
	Invalid UMETA(DisplayName = "None"),
	ViaAngle UMETA(DisplayName = "Via Angle"),
	ViaTimer UMETA(DisplayName = "Via Timer")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VISUALSTUDIOTEST_API UAttractionActorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	FTimerHandle RaycastTimerHandle;

	// The interval between each Raycast function call in seconds
	UPROPERTY(EditAnywhere, Category = "Raycast")
	float RaycastInterval = 0.8f; 

public:	
	// Sets default values for this component's properties
	UAttractionActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Raycast();

	void RaycastTimer();

	void StartAttracting();

	void StopAttracting();

	void AngleDistanceCalculation();

	class UAttractableActorComponent* GetAttractableActorComponent(AActor* Actor) const;

	void ResetAttractedActors(TArray<AActor*>& AttractedActors);

	AVisualStudioTestCharacter* m_ThirdPersonCharacter = nullptr;

	bool m_bIsAttractInputPressed = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAttractionMode m_CurrentAttractionMode;

	EAttractionMode GetAttractionMode() { return m_CurrentAttractionMode; }
	void SetAttractionMode(EAttractionMode AttractionMode) { m_CurrentAttractionMode = AttractionMode; }

	void ToggleAttractionMode();

};

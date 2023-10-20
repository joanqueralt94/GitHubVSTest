// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VisualStudioTestCharacter.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class IInventoryInterface;
class AInventoryActor;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VISUALSTUDIOTEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EEndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AVisualStudioTestCharacter* ThirdPersonCharacter;

	TArray<IInventoryInterface*> m_ActorsInInventory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int m_InventorySize = 0;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int m_CountInInventory = 0;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int m_SpawnDistance = 0;

	void DropActor();

	void PickUpActor();

	float Health = 100;
	float MaxHealth = 100;

	bool bGameEnded = false;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	UPROPERTY()
	class UPlayerHUD* PlayerHUD;

	FTimerHandle FinishLevelTimerHandle;

	void FinishLevel();

	void StartFinishLevelTimer(float DelayInSeconds);
};

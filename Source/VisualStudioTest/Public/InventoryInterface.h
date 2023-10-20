// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VISUALSTUDIOTEST_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool GetIsInInventory() const = 0;
	virtual void SetIsInInventory(bool bValue) = 0;

	virtual void SetActorHiddenInGame(bool bValue) = 0;
	virtual void SetActorLocation(const FVector& NewLocation, bool bSweep = false) = 0;
	virtual void SetActorRotation(const FRotator& NewRotation, ETeleportType Teleport = ETeleportType::None) = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TestObject.generated.h"

/**
 * 
 */

class UStaticMeshComponent;

UCLASS()
class VISUALSTUDIOTEST_API UTestObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent = nullptr;
};

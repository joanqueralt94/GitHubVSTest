// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VisualStudioTestCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryActor.generated.h"

UCLASS()
class VISUALSTUDIOTEST_API AInventoryActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USceneComponent* DefaultRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* m_MyStaticMesh;

	AVisualStudioTestCharacter* ThirdPersonCharacter;

	UFUNCTION()
	void OnMeshActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

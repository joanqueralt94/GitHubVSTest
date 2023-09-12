// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VisualStudioTestCharacter.h"
#include "GameFramework/Actor.h"
#include "MeshActor.generated.h"

class UPrimitiveComponent;
class AActor;

UCLASS()
class VISUALSTUDIOTEST_API AMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

public:

	AVisualStudioTestCharacter* ThirdPersonCharacter;

	UPROPERTY()
	USceneComponent* DefaultRootComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

	//Create new component that inherits from UActorComponent, add it to the MeshActor and encapsulate all the attachment logic in there.
	//class UAttractableComponent -> llògica de atracció + discriminació

	//Array de pointers a Actors que li faig el raycast

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "RayCast")
	float AttractSpeed = 20.f;

	bool IsAttracting = false;
};

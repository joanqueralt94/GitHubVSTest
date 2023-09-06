// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshActor.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AMeshActor::AMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponentName");
}

// Called when the game starts or when spawned
void AMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


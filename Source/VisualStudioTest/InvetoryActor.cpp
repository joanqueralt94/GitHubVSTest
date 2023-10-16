// Fill out your copyright notice in the Description page of Project Settings.


#include "InvetoryActor.h"

// Sets default values
AInvetoryActor::AInvetoryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	RootComponent = DefaultRootComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponentName");
	StaticMeshComponent->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));

	if (MeshAsset.Succeeded())
	{
		m_MyStaticMesh = MeshAsset.Object;
		StaticMeshComponent->SetStaticMesh(m_MyStaticMesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error Loading Static Mesh"));
	}


}

// Called when the game starts or when spawned
void AInvetoryActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInvetoryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


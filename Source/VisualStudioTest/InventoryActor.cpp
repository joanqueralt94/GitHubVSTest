// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActor.h"
#include "kismet/gameplaystatics.h"

// Sets default values
AInventoryActor::AInventoryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	RootComponent = DefaultRootComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponentName");
	StaticMeshComponent->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AInventoryActor::OnMeshActorHit);
	StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

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
void AInventoryActor::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AInventoryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInventoryActor::OnMeshActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

void AInventoryActor::SetActorHiddenInGame(bool bValue)
{
	Super::SetActorHiddenInGame(bValue);
}

void AInventoryActor::SetActorLocation(const FVector& NewLocation, bool bSweep)
{
	Super::SetActorLocation(NewLocation, bSweep);
}

void AInventoryActor::SetActorRotation(const FRotator& NewRotation, ETeleportType Teleport)
{
	Super::SetActorRotation(NewRotation, Teleport);
}


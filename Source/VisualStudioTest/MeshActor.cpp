// Fill out your copyright notice in the Description page of Project Settings.

#include "MeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "kismet/gameplaystatics.h"
#include "AttractableActorComponent.h"

// Sets default values
AMeshActor::AMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent= CreateDefaultSubobject<USceneComponent>("DefaultRootComponent");
	RootComponent = DefaultRootComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponentName");
	StaticMeshComponent->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	CollisionBox->SetCollisionProfileName("OverlapOnlyPawn");
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeshActor::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AMeshActor::OnOverlapEnd);

	AttractableActorComp = CreateDefaultSubobject<UAttractableActorComponent>(TEXT("AttractableActorComponent"));

}

// Called when the game starts or when spawned
void AMeshActor::BeginPlay()
{
	Super::BeginPlay();

	ThirdPersonCharacter = Cast<AVisualStudioTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

// Called every frame
void AMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeshActor::OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ThirdPersonCharacter && ThirdPersonCharacter == OtherActor)
	{
		//StaticMeshComponent->SetSimulatePhysics(false);
	}
}

void AMeshActor::OnOverlapEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ThirdPersonCharacter && ThirdPersonCharacter == OtherActor)
	{
		
	}
}
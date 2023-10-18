// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "InventoryActor.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	PlayerHUDClass = nullptr;
	PlayerHUD = nullptr;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ThirdPersonCharacter = Cast<AVisualStudioTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(PlayerController, PlayerHUDClass);
		check(PlayerHUD);
		PlayerHUD->AddToPlayerScreen();
	}

	for (int32 i = 0; i < m_InventorySize; i++)
	{
		FVector Location = FVector::ZeroVector;
		FRotator Rotation = ThirdPersonCharacter->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		UClass* InventoryActorClass = AInventoryActor::StaticClass();
		AInventoryActor* SpawnedActorRef = GetWorld()->SpawnActor<AInventoryActor>(InventoryActorClass, Location, Rotation, SpawnParams);
		SpawnedActorRef->SetActorHiddenInGame(true);
		m_ActorsInInventory.Push(SpawnedActorRef);
		SpawnedActorRef->SetIsInInventory(true);
		m_CountInInventory++;
	}

	PlayerHUD->SetInventoryCountNumber(m_CountInInventory);
	
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EEndPlayReason)
{
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}

	Super::EndPlay(EEndPlayReason);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::DropActor()
{
	if (m_ActorsInInventory.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "INVENTORY IS EMPTY");
	}
	else
	{
		for (int i = 0; i < m_ActorsInInventory.Num(); i++)
		{
			if (m_ActorsInInventory[i]->GetIsInInventory())
			{
				FVector TempInvetoryLocation = ThirdPersonCharacter->GetActorLocation() + ThirdPersonCharacter->GetActorForwardVector() * m_SpawnDistance;

				m_ActorsInInventory[i]->SetActorLocation(TempInvetoryLocation);
				m_ActorsInInventory[i]->SetActorHiddenInGame(false);
				m_ActorsInInventory[i]->SetIsInInventory(false);
				m_CountInInventory--;
				break;
			}
		}

		PlayerHUD->SetInventoryCountNumber(m_CountInInventory);

		Health = Health - 20;
		PlayerHUD->SetHealth(Health, MaxHealth);

		if (Health == 0)
		{
			bGameEnded = true;
			PlayerHUD->ShowGameOverText(bGameEnded);
			StartFinishLevelTimer(2.0f);
		}
	}
}


void UInventoryComponent::PickUpActor()
{
	if (m_CountInInventory < m_InventorySize)
	{
		for (int i = 0; i < m_ActorsInInventory.Num(); i++)
		{
			if (!m_ActorsInInventory[i]->GetIsInInventory())
			{
				FVector Location = FVector::ZeroVector;
				m_ActorsInInventory[i]->SetActorLocation(Location);
				m_ActorsInInventory[i]->SetActorHiddenInGame(true);
				m_ActorsInInventory[i]->SetIsInInventory(true);
				m_CountInInventory++;
				break;
			}
		}

		PlayerHUD->SetInventoryCountNumber(m_CountInInventory);

		Health = Health + 20;
		PlayerHUD->SetHealth(Health, MaxHealth);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "INVENTORY IS FULL");
	}
}

void UInventoryComponent::FinishLevel()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Type::Quit, false);
}

void UInventoryComponent::StartFinishLevelTimer(float DelayInSeconds)
{
	GetWorld()->GetTimerManager().SetTimer(FinishLevelTimerHandle, this, &UInventoryComponent::FinishLevel, DelayInSeconds, false);
}


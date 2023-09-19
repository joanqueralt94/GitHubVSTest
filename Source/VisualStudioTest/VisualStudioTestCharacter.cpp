// Copyright Epic Games, Inc. All Rights Reserved.

#include "VisualStudioTestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "MeshActor.h"
#include "AttractableActorComponent.h"

//////////////////////////////////////////////////////////////////////////
// AVisualStudioTestCharacter

AVisualStudioTestCharacter::AVisualStudioTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AVisualStudioTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Attract", IE_Pressed, this, &AVisualStudioTestCharacter::StartAttracting);
	PlayerInputComponent->BindAction("Attract", IE_Released, this, &AVisualStudioTestCharacter::StopAttracting);

	PlayerInputComponent->BindAction("Spawn", IE_Pressed, this, &AVisualStudioTestCharacter::SpawnObject);


	PlayerInputComponent->BindAxis("MoveForward", this, &AVisualStudioTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVisualStudioTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AVisualStudioTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AVisualStudioTestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AVisualStudioTestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AVisualStudioTestCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AVisualStudioTestCharacter::OnResetVR);
}

void AVisualStudioTestCharacter::OnResetVR()
{
	// If VisualStudioTest is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in VisualStudioTest.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVisualStudioTestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AVisualStudioTestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AVisualStudioTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AVisualStudioTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AVisualStudioTestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AVisualStudioTestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void AVisualStudioTestCharacter::SpawnObject()
{
	FVector Location;
	FRotator Rotation; 

	Location = GetActorLocation() + GetActorForwardVector()*SpawnDistance;
	Rotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, SpawnParams);

}

void AVisualStudioTestCharacter::RayCast()
{
	TArray<FHitResult> m_OutHitArray;
	m_OutHitArray.Reset();

	FVector Start = FollowCamera->GetComponentLocation();
	const FVector ForwardVector = FollowCamera->GetForwardVector();

	Start = Start + (ForwardVector * CameraBoom->TargetArmLength);

	const FVector End = Start + (ForwardVector * RaycastDistance);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	const UWorld* MyWorld = GetWorld();
	check(MyWorld != nullptr);

	static constexpr bool bPersistentLines = false;
	static constexpr float LifeTime = 2.f;
	static constexpr uint8 DepthPriority = 0;
	static constexpr float Thickness = 2.f;

	DrawDebugLine(MyWorld, Start, End, FColor::Yellow, bPersistentLines, LifeTime, DepthPriority, Thickness);

	static constexpr ECollisionChannel TraceChannel = ECC_Visibility;

	const bool bIsHitArray = MyWorld->LineTraceMultiByChannel(m_OutHitArray, Start, End, TraceChannel, CollisionParams);

	if (bIsHitArray)
	{
		for (FHitResult& HitResult : m_OutHitArray)
		{
			AActor* ActorHit = HitResult.GetActor();
			if (ActorHit != nullptr && ActorHit->ActorHasTag(TEXT("Attractable")))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "Actor Hit: " + ActorHit->GetName());
				UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *ActorHit->GetName());

				if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(ActorHit))
				{
					m_AttractedActors.AddUnique(ActorHit);
					AttractableComponent->StartAttracting(this);
				}
			}
		}
	}
}

void AVisualStudioTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAttractableActorComponent* AVisualStudioTestCharacter::GetAttractableActorComponent(AActor* Actor) const
{
	if (Actor != nullptr)
	{
		UActorComponent* ActorComponent = Actor->GetComponentByClass(UAttractableActorComponent::StaticClass());

		return Cast<UAttractableActorComponent>(ActorComponent);
	}
	return nullptr;
}

void AVisualStudioTestCharacter::StartAttracting()
{
	RayCast();
}

void AVisualStudioTestCharacter::StopAttracting()
{
	for (AActor* Actor : m_AttractedActors)
	{
		if (UAttractableActorComponent* AttractableComponent = GetAttractableActorComponent(Actor))
		{
			AttractableComponent->StartAttracting(nullptr);
		}
	}

	m_AttractedActors.Reset();
}
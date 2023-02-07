// Copyright Epic Games, Inc. All Rights Reserved.

#include "DestinyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Destiny/Public/Interact/InteractableObject.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Cameras/BaseCamera.h"
#include "Cameras/CameraInterface.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// ADestinyCharacter


ADestinyCharacter::ADestinyCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 210.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(RootComponent);

	ObjectAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ObjectAttachPoint"));
	ObjectAttachPoint->SetupAttachment(FPCamera);
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ADestinyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FindNearestCamera();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADestinyCharacter::FindNearestCamera()
{
	const TSubclassOf<ACameraActor> ClassToFind = ACameraActor::StaticClass();

	TArray<AActor*> CamerasInLevel;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, CamerasInLevel);

	float NearCameraDistance = FVector::Distance(CamerasInLevel[0]->GetActorLocation(), GetActorLocation());
	
	for(int i = 0; i < CamerasInLevel.Num(); i++)
	{
		const float CameraDistance = FVector::Distance(CamerasInLevel[i]->GetActorLocation(), GetActorLocation());
		if(NearCameraDistance > CameraDistance)
		{
			NearCameraDistance = CameraDistance;
			
			CurrentCameraActor = Cast<ABaseCamera>(CamerasInLevel[i]);
			if(CurrentCameraActor)
				CurrentCameraActor->Execute_ActivateCamera(CurrentCameraActor ,this);
		}
	}

}

//////////////////////////////////////////////////////////////////////////
// Input



void ADestinyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ADestinyCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADestinyCharacter::EndSprint);

		// Focus
		EnhancedInputComponent->BindAction(TryFocusAction, ETriggerEvent::Completed, this, &ADestinyCharacter::SearchObjectToInteract);
		
		EnhancedInputComponent->BindAction(TryCancelFocusAction, ETriggerEvent::Completed, this, &ADestinyCharacter::CancelFocus);
		

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADestinyCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADestinyCharacter::Look);

	}

}

void ADestinyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FVector ForwardDirection = CurrentCameraActor->GetCameraForwardVector();
		const FVector RightDirection = CurrentCameraActor->GetCameraRightVector();

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADestinyCharacter::Sprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void ADestinyCharacter::EndSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void ADestinyCharacter::SearchObjectToInteract(const FInputActionValue& Value)
{
	const FCollisionShape MySphere = FCollisionShape::MakeBox(BoxSize);
	TArray<FHitResult> OutResults;
	
	GetWorld()->SweepMultiByChannel(OutResults, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_Visibility, MySphere);
	for(FHitResult Hit : OutResults)
	{
		if(AActor* HitActor = Hit.GetActor())
		{
			ObjectToInteract = Cast<AInteractableObject>(Hit.GetActor());

			if(ObjectToInteract)
			{
				if(ObjectToInteract->Implements<UInteractInterface>())
				{
					IInteractInterface::Execute_StartFocus(ObjectToInteract, this);
					return;
				}
			}
		}

	}
}

void ADestinyCharacter::CancelFocus(const FInputActionValue& Value)
{
	if (ObjectToInteract)
	{
		if(ObjectToInteract->Implements<UInteractInterface>())
		{
			IInteractInterface::Execute_StopFocus(ObjectToInteract, this);
			ObjectToInteract =	nullptr;
		}
	}
}

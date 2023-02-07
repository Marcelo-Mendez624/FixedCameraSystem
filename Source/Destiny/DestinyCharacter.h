// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DestinyCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class AInteractableObject;
class ABaseCamera;
class UCameraComponent;

UCLASS(config=Game)
class ADestinyCharacter : public ACharacter
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FPCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ObjectAttachPoint;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	/** Focus Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TryFocusAction;

	/** Cancel Focus Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TryCancelFocusAction;

	// Base walk speed
	UPROPERTY(EditDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseWalkSpeed;

	// Sprint speed
	UPROPERTY(EditDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeed;

	// Function to find the closest camera
	void FindNearestCamera();

	// Box size for find objects
	UPROPERTY(EditDefaultsOnly)
	FVector BoxSize;

public:
	ADestinyCharacter();

	virtual void BeginPlay() override;

	// Object to interact
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AInteractableObject* ObjectToInteract;

	// Current active camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABaseCamera* CurrentCameraActor;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/* Called for start sprint */
	void Sprint(const FInputActionValue& Value);

	/* Called for stop sprint */
	void EndSprint(const FInputActionValue& Value);

	/* Called for search object */
	void SearchObjectToInteract(const FInputActionValue& Value);

	/* Called to cancel focus to object */
	void CancelFocus(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Return FVector to Object attach point
	UFUNCTION(BlueprintGetter)
	FORCEINLINE USceneComponent* GetObjectAttachPoint() const {return ObjectAttachPoint; }

};


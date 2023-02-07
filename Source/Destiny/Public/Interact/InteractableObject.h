// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact/InteractInterface.h"
#include "InteractableObject.generated.h"

class UCameraComponent;
class UBoxComponent;


// you can move this enum to an empty class to improve optimization 
UENUM(BlueprintType)
enum class EEventType : uint8
{
	ETotemFound	UMETA(DisplayName = "TotemFound"),
	EClueFound		UMETA(DisplayName = "ClueFound"),

	// you can add more event if this is required
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventTrigger, EEventType, EventType);

UCLASS()
class DESTINY_API AInteractableObject : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	// Called when the player interacts with the object
	virtual void StartFocus_Implementation(ACharacter* Caller) override;

	// Called when the player stops the interaction with the object
	virtual void StopFocus_Implementation(ACharacter* Caller) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Base object mesh
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* BaseMesh;

	// Object type event
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HorrorEvents)
	EEventType TypeEvent;

private:
	// Object initial position to reallocate after stop focus 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector ObjectInitialPosition;

public:
	// You can make a broadcast to delegate horror events in blueprints
	UPROPERTY(BlueprintAssignable)
	FOnEventTrigger OnEventTrigger;

	
};

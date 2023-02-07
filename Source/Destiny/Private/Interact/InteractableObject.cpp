// Fill out your copyright notice in the Description page of Project Settings.


#include "Destiny/Public/Interact/InteractableObject.h"

#include "Destiny/DestinyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	ObjectInitialPosition = GetActorLocation();
	
}

// Called when the player interacts with the object
void AInteractableObject::StartFocus_Implementation(ACharacter* Caller)
{
	OnEventTrigger.Broadcast(TypeEvent);
}

// Called when the player stop interacts with the object
void AInteractableObject::StopFocus_Implementation(ACharacter* Caller)
{

}




// Fill out your copyright notice in the Description page of Project Settings.


#include "Cameras/BaseCamera.h"

#include "Destiny/DestinyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCamera::ABaseCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	
	SetCameraForwardVector();
	
	SetCameraRightVector();
	
}

void ABaseCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(CameraTargetActor && bOrientedToCharacter)
	{
		const FRotator NewActorRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),CameraTargetActor->GetActorLocation());
		
		SetActorRotation(NewActorRotation);
	}
}

void ABaseCamera::ActivateCamera_Implementation(AActor* TargetActor)
{
	bIsCameraActive = true;
	
	SetActorTickEnabled(true);
	
	CameraTargetActor = TargetActor;

	if(ADestinyCharacter* Character = Cast<ADestinyCharacter>(TargetActor))
		Character->CurrentCameraActor = this;

	UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(this);
}

void ABaseCamera::DeactivateCamera_Implementation()
{
	bIsCameraActive = false;
	
	SetActorTickEnabled(false);
}

void ABaseCamera::SetCameraRightVector()
{
	if(bConvertRightVectorToForwardVector)
 	{
 		CameraRightVector = bInvertForwardVector ? GetActorForwardVector()* -1 : GetActorForwardVector();
 	}
 	
 	CameraRightVector = bInvertRightVector ? GetActorRightVector()* -1 : GetActorRightVector();
}

void ABaseCamera::SetCameraForwardVector()
{
	if(bConvertRightVectorToForwardVector)
 	{
 		CameraForwardVector = bInvertRightVector ? GetActorRightVector()* -1 : GetActorRightVector();
 	}
 	
 	CameraForwardVector = bInvertForwardVector ? GetActorForwardVector()* -1 : GetActorForwardVector();
}


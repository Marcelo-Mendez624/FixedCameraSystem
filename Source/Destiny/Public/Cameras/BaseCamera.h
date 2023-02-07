// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraInterface.h"
#include "Camera/CameraActor.h"
#include "BaseCamera.generated.h"

/**
 * 
 */

class ACameraActor;

UCLASS()
class DESTINY_API ABaseCamera : public ACameraActor, public ICameraInterface
{
	GENERATED_BODY()

public:
	ABaseCamera();

	virtual void BeginPlay() override;
	
private:
	virtual void Tick(float DeltaSeconds) override;


protected:
	UPROPERTY(VisibleAnywhere)
	AActor* CameraTargetActor;
	
public:
	virtual void ActivateCamera_Implementation(AActor* TargetActor) override;
	virtual void DeactivateCamera_Implementation() override;

private:
	void SetCameraRightVector();
	void SetCameraForwardVector();

	// Bool to allow camera follow player movement
	UPROPERTY(EditAnywhere, Category = "CameraSettings", meta = (AllowPrivateAccess = "true"))
	bool bOrientedToCharacter;

	// Bool to invert camera forward vector
	UPROPERTY(EditAnywhere, Category = "CameraSettings", meta = (AllowPrivateAccess = "true"))
	bool bInvertForwardVector;

	// Bool to invert camera right vector
	UPROPERTY(EditAnywhere, Category = "CameraSettings", meta = (AllowPrivateAccess = "true"))
	bool bInvertRightVector;

	// Switch forward and right vectors
	UPROPERTY(EditAnywhere, Category = "CameraSettings", meta = (AllowPrivateAccess = "true"))
	bool bConvertRightVectorToForwardVector;
	
	UPROPERTY(VisibleAnywhere, Category = "CameraSettings", meta = (AllowPrivateAccess = "true"))
	FVector CameraRightVector;
	
	UPROPERTY(VisibleAnywhere, Category = "CameraSettings", meta = (AllowPrivateAccess = "true"))
	FVector CameraForwardVector;

	UPROPERTY(VisibleAnywhere, Category = "CameraSettings", meta = (AllowPrivateAccess = "true"))
	bool bIsCameraActive;
	
public:
	UFUNCTION(BlueprintCallable)
	FVector GetCameraRightVector() const { return  CameraRightVector; }
	
	UFUNCTION(BlueprintCallable)
	FVector GetCameraForwardVector() const { return  CameraForwardVector; };
	
	UFUNCTION(BlueprintCallable)
	bool GetCameraIsActive() const { return  bIsCameraActive; };
	
};

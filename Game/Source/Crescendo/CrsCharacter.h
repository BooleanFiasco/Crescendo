// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "CrsNavPointComponent.h"
#include "CrsCharacter.generated.h"

UCLASS()
class CRESCENDO_API ACrsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UCrsNavPointComponent* CurrentPoint;
	UCrsNavPointComponent* DestinationPoint;

	// Sets default values for this character's properties
	ACrsCharacter();

	// Begin ACharacter interface
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End ACharacter interface

	void Move(ENavDirection::Type Direction);
	void QueueMove(ESwipeDirection::Type Direction);
	void MoveFinished();
	void FaceDirection(ENavDirection::Type Direction);
	bool IsMoving() const { return DestinationPoint != nullptr; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Animations)
	UAnimMontage* MoveMontage;
	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera )
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	ESwipeDirection::Type QueuedMove;
};

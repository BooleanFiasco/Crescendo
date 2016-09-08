// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "CrsCharacter.generated.h"

UCLASS()
class CRESCENDO_API ACrsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrsCharacter();

	// Begin ACharacter interface
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End ACharacter interface

	void Move();	// Temp
	void QueuedMove();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Animations)
	UAnimMontage* MoveMontage;
	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera )
		class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CameraBoom;

	bool bQueuedMove;
};

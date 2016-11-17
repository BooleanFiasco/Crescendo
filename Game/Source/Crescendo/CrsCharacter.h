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
	// Sets default values for this character's properties
	ACrsCharacter(const FObjectInitializer& ObjectInitializer);

	// Begin ACharacter interface
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End ACharacter interface

	UFUNCTION(BlueprintCallable, Category=Crescendo)
	void ScriptMove(ENavDirection::Type Direction, int NumPoints, UCrsNavPointComponent* Destination);
	void Move(ENavDirection::Type Direction);
	void QueueMove(ENavDirection::Type Direction);
	void MoveFinished();
	void FaceDirection(ENavDirection::Type Direction);
	bool IsMoving() const { return DestinationPoint != nullptr; }

protected:
	void FloorMove(ENavDirection::Type Direction);
	void WallMove(ENavDirection::Type Direction);
	void FloorTransition(ENavDirection::Type Direction);
	void WallTransition(ENavDirection::Type Direction);

public:
	UPROPERTY(Transient, BlueprintReadWrite, Category=Crescendo)
	UCrsNavPointComponent* CurrentPoint;
	UCrsNavPointComponent* DestinationPoint;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Animations)
	UAnimMontage* MoveMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbUpToWallMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbDownToWallMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbDownToWallBackwardMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbUpToFloorMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbDownToFloorMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbLeftMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbLeftConvexMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbLeftConcaveMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbRightMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbRightConvexMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbRightConcaveMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbUpMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimMontage* ClimbDownMontage;
	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera )
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	ENavDirection::Type QueuedMove;

	ENavDirection::Type ScriptedMove;
	int ScriptedMoves;
};

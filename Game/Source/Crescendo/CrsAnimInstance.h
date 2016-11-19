// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "CrsCharacter.h"
#include "CrsAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRESCENDO_API UCrsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCrsAnimInstance();

	// Begin UAnimInstance interface
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	// End UAnimInstance interface

	UFUNCTION()
	void AnimNotify_FootPlanted();

	UFUNCTION()
	void AnimNotify_MoveFinished();

	UFUNCTION(BlueprintCallable, Category=Crescendo)
	ACrsCharacter* GetCharacterOwner() const;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Transient, Category=StateFlags)
	bool bIsClimbing;
	
};

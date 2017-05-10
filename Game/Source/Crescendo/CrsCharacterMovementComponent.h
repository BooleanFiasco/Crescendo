// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "CrsCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CRESCENDO_API UCrsCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	
public:
	static const int CustomMovementMode_Climbing = 0;

};

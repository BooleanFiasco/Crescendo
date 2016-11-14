// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsCharacterMovementComponent.h"

void UCrsCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
		case CustomMovementMode_Climbing:
			PhysFlying(deltaTime, Iterations);
			break;
		default:
			Super::PhysCustom(deltaTime, Iterations);
	}
}

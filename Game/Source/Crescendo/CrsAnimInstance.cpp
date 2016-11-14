// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsAnimInstance.h"
#include "CrsCharacter.h"

UCrsAnimInstance::UCrsAnimInstance() {
	bIsClimbing = false;
}

void UCrsAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Char = GetCharacterOwner();
	if (Char != nullptr && Char->CurrentPoint != nullptr)
	{
		bIsClimbing = Char->CurrentPoint->IsWall();
	}
}

void UCrsAnimInstance::AnimNotify_FootPlanted()
{
	AnimNotify_MoveFinished();
}

void UCrsAnimInstance::AnimNotify_MoveFinished()
{
	auto Char = GetCharacterOwner();
	if (Char == nullptr) return;

	Char->MoveFinished();
}

ACrsCharacter* UCrsAnimInstance::GetCharacterOwner() const
{
	if (GetOwningActor() == nullptr) return nullptr;

	return Cast<ACrsCharacter>(GetOwningActor());
}

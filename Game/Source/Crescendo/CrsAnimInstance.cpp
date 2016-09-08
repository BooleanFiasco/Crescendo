// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsAnimInstance.h"

UCrsAnimInstance::UCrsAnimInstance() {
	bIsClimbing = false;
}

void UCrsAnimInstance::AnimNotify_FootPlanted()
{
	auto Char = GetCharacterOwner();
	if (Char == nullptr) return;

	Char->QueuedMove();
}

ACrsCharacter* UCrsAnimInstance::GetCharacterOwner() const
{
	if (GetOwningActor() == nullptr) return nullptr;

	return CastChecked<ACrsCharacter>(GetOwningActor());
}

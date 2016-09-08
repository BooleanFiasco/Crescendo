// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsCharacter.h"
#include "CrsAnimInstance.h"

// Sets default values
ACrsCharacter::ACrsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bQueuedMove = false;
}

// Called when the game starts or when spawned
void ACrsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrsCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ACrsCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ACrsCharacter::Move()
{
	if (bQueuedMove) return;

	auto AnimInst = CastChecked<UCrsAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInst->Montage_IsPlaying(MoveMontage))
	{
		FName MoveSection = AnimInst->Montage_GetCurrentSection(MoveMontage);
		FName NewSection;
		if (MoveSection == TEXT("Left_Outro"))
		{
			NewSection = TEXT("Right");
		}
		else if (MoveSection == TEXT("Right_Outro"))
		{
			NewSection = TEXT("Left");
		}
		else
		{
			bQueuedMove = true;
			return;
		}

		AnimInst->Montage_Play(MoveMontage);
		AnimInst->Montage_JumpToSection(NewSection);
	}
	else
	{
		AnimInst->Montage_Play(MoveMontage);
	}
}

void ACrsCharacter::QueuedMove()
{
	if (bQueuedMove)
	{
		bQueuedMove = false;
		Move();
	}
}

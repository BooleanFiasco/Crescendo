// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsCharacter.h"
#include "CrsAnimInstance.h"
#include "CrsTile.h"
#include "CrsPlayerController.h"

// Sets default values
ACrsCharacter::ACrsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	QueuedMove = ESwipeDirection::None;
}

// Called when the game starts or when spawned
void ACrsCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Find nearest nav point and save it
	auto Cap = GetCapsuleComponent();
	float HalfHeight = Cap->GetScaledCapsuleHalfHeight();
	float Radius = Cap->GetScaledCapsuleRadius();
	FVector Loc = GetActorLocation() - (FVector::UpVector * HalfHeight);

	float BestDistSq = 999999.0f;
	UCrsNavPointComponent* BestPoint = nullptr;
	TArray<FOverlapResult> Results;
	if (GetWorld()->OverlapMultiByObjectType(Results, GetActorLocation(), GetActorQuat(), FCollisionObjectQueryParams::AllObjects, FCollisionShape::MakeCapsule(Radius, HalfHeight * 2.0f)))
	{
		for (auto& Result : Results)
		{
			if (Result.GetActor() == nullptr || !Result.GetActor()->IsA(ACrsTile::StaticClass()))
			{
				continue;
			}

			// Test all valid points
			auto Tile = CastChecked<ACrsTile>(Result.GetActor());
			for (auto& Point : Tile->GetNavPoints())
			{
				if (!Point->IsEnabled()) continue;

				float DistSq = FVector::DistSquared(Point->GetComponentLocation(), Loc);
				if (DistSq < BestDistSq)
				{
					BestDistSq = DistSq;
					BestPoint = Point;
				}
			}
		}

		if (BestPoint != nullptr)
		{
			CurrentPoint = BestPoint;
		}
		else
		{
			// TODO: This should show a warning, and probably and error if you try to save the map in this state.
		}
	}
	
}

// Called every frame
void ACrsCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (CurrentPoint != nullptr)
	{
		DrawDebugSphere(GetWorld(), CurrentPoint->GetComponentLocation(), 16.0f, 16, FColor::Red, false, 0.0f);
	}

	if (DestinationPoint != nullptr)
	{
		DrawDebugSphere(GetWorld(), DestinationPoint->GetComponentLocation(), 24.0f, 16, FColor::Green, false, 0.0f);
	}
}

// Called to bind functionality to input
void ACrsCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ACrsCharacter::Move(ENavDirection::Type Direction)
{
	auto AnimInst = CastChecked<UCrsAnimInstance>(GetMesh()->GetAnimInstance());
	
	FaceDirection(Direction);
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
			UE_LOG(LogCrs, Warning, TEXT("Trying to make a new move before previous animation has reached the foot plant section!"));
			ensure(false);
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

void ACrsCharacter::QueueMove(ESwipeDirection::Type Direction)
{
	QueuedMove = Direction;
}

void ACrsCharacter::MoveFinished()
{
	CurrentPoint = DestinationPoint;
	DestinationPoint = nullptr;

	if (QueuedMove != ESwipeDirection::None)
	{
		auto PC = CastChecked<ACrsPlayerController>(this->Controller);
		PC->TrySwipe(QueuedMove);
		QueuedMove = ESwipeDirection::None;
	}
}

void ACrsCharacter::FaceDirection(ENavDirection::Type Direction)
{
	FVector DirVec = FVector::ForwardVector;
	switch (Direction)
	{
		case ENavDirection::Forward:
			DirVec = FVector::ForwardVector;
			break;
		case ENavDirection::Back:
			DirVec = -FVector::ForwardVector;
			break;
		case ENavDirection::Right:
			DirVec = FVector::RightVector;
			break;
		case ENavDirection::Left:
			DirVec = -FVector::RightVector;
			break;
		default:
			DirVec = FVector::ForwardVector;
			break;
	}

	FRotator temp = GetActorRotation();
	FRotator newRot = FRotator(0.0f, FMath::RadiansToDegrees(DirVec.HeadingAngle()), 0.0f);

	SetActorRotation(newRot);
}

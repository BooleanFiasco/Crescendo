// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsPlayerController.h"
#include "CrsCharacter.h"

ACrsPlayerController::ACrsPlayerController()
{
	memset(TouchLocations, 0, sizeof(FVector) * EKeys::NUM_TOUCH_KEYS);
	memset(TouchStartLocations, 0, sizeof(FVector) * EKeys::NUM_TOUCH_KEYS);

	SwipeDistance = 64.0f;
	AutoSwipeDistance = 128.0f;
	DiagonalSwipeMin = 15.0f;
	DiagonalSwipeMax = 75.0f;
	VerticalSwipeAngle = 60.0f;
	HorizontalSwipeAngle = 30.0f;
}

void ACrsPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ACrsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind touch events
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ACrsPlayerController::OnTouchBegin);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ACrsPlayerController::OnTouchRepeat);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &ACrsPlayerController::OnTouchEnd);
}

void ACrsPlayerController::OnTouchBegin(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	TouchStartLocations[FingerIndex] = Location;
	TouchLocations[FingerIndex] = Location;
}

void ACrsPlayerController::OnTouchRepeat(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchStartLocations[FingerIndex].IsZero()) return;

	TouchLocations[FingerIndex] = Location;
	if (CheckForSwipe(FingerIndex, EInputEvent::IE_Repeat))
	{
		TouchStartLocations[FingerIndex] = FVector::ZeroVector;
		TouchLocations[FingerIndex] = FVector::ZeroVector;
	}
}

void ACrsPlayerController::OnTouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchStartLocations[FingerIndex].IsZero()) return;

	TouchLocations[FingerIndex] = Location;
	if (CheckForSwipe(FingerIndex, EInputEvent::IE_Released))
	{
		TouchStartLocations[FingerIndex] = FVector::ZeroVector;
		TouchLocations[FingerIndex] = FVector::ZeroVector;
	}
}

bool ACrsPlayerController::CheckForSwipe(const ETouchIndex::Type FingerIndex, const EInputEvent TouchType)
{
	FVector SwipeDirection = TouchLocations[FingerIndex] - TouchStartLocations[FingerIndex];
	float SwipeDist = SwipeDirection.Size2D();
	SwipeDirection = SwipeDirection.GetSafeNormal2D();
	float CheckDist = TouchType == EInputEvent::IE_Released ? SwipeDistance : AutoSwipeDistance;
	if (SwipeDist < CheckDist) {
		return false;
	}

	// Determine direction relative to a vector straight up in screenspace
	float UpAngle = -FVector::DotProduct(FVector::RightVector, SwipeDirection);
	float RightAngle = FVector::DotProduct(FVector::ForwardVector, SwipeDirection);

	// Cache
	float DAngMin = FMath::Cos(FMath::DegreesToRadians(DiagonalSwipeMin));
	float DAngMax = FMath::Cos(FMath::DegreesToRadians(DiagonalSwipeMax));
	float HAng = FMath::Cos(FMath::DegreesToRadians(HorizontalSwipeAngle));
	float VAng = FMath::Cos(FMath::DegreesToRadians(VerticalSwipeAngle));

	// We try the diagonal swipes first as they're the most important, then we do the
	// horizontal and vertical ones after. Eventually we'll probably want to swap the
	// priority of specific directions based on what is available to that tile.
	auto Result = ESwipeDirection::None;
	if (UpAngle <= DAngMin && UpAngle >= DAngMax)
	{
		if (RightAngle > 0)
		{
			Result = ESwipeDirection::UpRight;
		}
		else
		{
			Result = ESwipeDirection::UpLeft;
		}
	}
	else if (UpAngle <= -DAngMax && UpAngle >= -DAngMin)
	{
		if (RightAngle > 0)
		{
			Result = ESwipeDirection::DownRight;
		}
		else
		{
			Result = ESwipeDirection::DownLeft;
		}
	}

	bool bFoundSwipe = Result != ESwipeDirection::None;
	if (bFoundSwipe)
	{
		TrySwipe(Result);
	}

	return bFoundSwipe;
}

void ACrsPlayerController::TrySwipe(ESwipeDirection::Type SwipeDirection)
{
	if (GetCharacter() == nullptr) return;

	auto Char = CastChecked<ACrsCharacter>(GetCharacter());
	auto NavDir = UCrsNavPointComponent::SwipeToNavDirection(SwipeDirection);

	// If we're already moving we need to test against the *next* node in the chain rather than our current one
	if (Char->IsMoving())
	{
		
		Char->QueueMove(SwipeDirection);
		return;
	}

	
	auto Point = Char->CurrentPoint;
	if (Point != nullptr && Point->GetNavLink(NavDir)->LinkedPoint != nullptr)
	{
		Char->DestinationPoint = Point->GetNavLink(NavDir)->LinkedPoint;
		Char->Move(NavDir);
	}
}

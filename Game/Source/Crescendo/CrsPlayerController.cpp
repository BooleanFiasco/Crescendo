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
	VerticalSwipeAngle = 30.0f;
	HorizontalSwipeAngle = 60.0f;
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
	/*if (TouchStartLocations[FingerIndex].IsZero()) return;

	TouchLocations[FingerIndex] = Location;
	if (CheckForSwipe(FingerIndex, EInputEvent::IE_Repeat))
	{
		TouchStartLocations[FingerIndex] = FVector::ZeroVector;
		TouchLocations[FingerIndex] = FVector::ZeroVector;
	}*/
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

ENavDirection::Type ACrsPlayerController::CheckForMovementSwipe(const float& UpAngle, const float& RightAngle)
{
	auto Char = CastChecked<ACrsCharacter>(GetCharacter());
	auto Point = Char->IsMoving() ? Char->DestinationPoint : Char->CurrentPoint;
	if (Point == nullptr) return ENavDirection::Max;

	// Cache
	float DAngMin = FMath::Cos(FMath::DegreesToRadians(DiagonalSwipeMin));
	float DAngMax = FMath::Cos(FMath::DegreesToRadians(DiagonalSwipeMax));
	float HAng = FMath::Cos(FMath::DegreesToRadians(HorizontalSwipeAngle));
	float VAng = FMath::Cos(FMath::DegreesToRadians(VerticalSwipeAngle));

	// Test against directional links that this point actually has to make is easier to differentiate
	// between horizontal, vertical and diagonal swipes.
	if (Point->GetNavLink(ENavDirection::Forward)->LinkedPoint != nullptr)
	{
		auto MaxAng = Point->IsLinkCorner(ENavDirection::Forward) ? FMath::Cos(FMath::DegreesToRadians(90.0f + DiagonalSwipeMin)) : DAngMax;
		if (UpAngle <= DAngMin && UpAngle >= MaxAng && RightAngle > 0)
		{
			return ENavDirection::Forward;
		}
	}

	if (Point->GetNavLink(ENavDirection::Back)->LinkedPoint != nullptr)
	{
		auto MaxAng = Point->IsLinkCorner(ENavDirection::Back) ? FMath::Cos(FMath::DegreesToRadians(90.0f - DiagonalSwipeMin)) : -DAngMax;
		if (UpAngle <= MaxAng && UpAngle >= -DAngMin && RightAngle < 0)
		{
			return ENavDirection::Back;
		}
	}

	if (Point->GetNavLink(ENavDirection::Right)->LinkedPoint != nullptr)
	{
		auto MaxAng = Point->IsLinkCorner(ENavDirection::Right) ? FMath::Cos(FMath::DegreesToRadians(90.0f - DiagonalSwipeMin)) : -DAngMax;
		if (UpAngle <= MaxAng && UpAngle >= -DAngMin && RightAngle > 0)
		{
			return ENavDirection::Right;
		}
	}

	if (Point->GetNavLink(ENavDirection::Left)->LinkedPoint != nullptr)
	{
		auto MaxAng = Point->IsLinkCorner(ENavDirection::Left) ? FMath::Cos(FMath::DegreesToRadians(90.0f + DiagonalSwipeMin)) : DAngMax;
		if (UpAngle <= DAngMin && UpAngle >= MaxAng && RightAngle < 0)
		{
			return ENavDirection::Left;
		}
	}

	if (Point->GetNavLink(ENavDirection::Up)->LinkedPoint != nullptr)
	{
		if (UpAngle >= VAng)
		{
			return ENavDirection::Up;
		}
	}

	if (Point->GetNavLink(ENavDirection::Down)->LinkedPoint != nullptr)
	{
		if (UpAngle <= -VAng)
		{
			return ENavDirection::Down;
		}
	}

	// Didn't find any matching swipes + link directions
	return ENavDirection::Max;
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

	if (GetCharacter() == nullptr) return false;

	// Determine direction relative to a vector straight up in screenspace
	float UpAngle = -FVector::DotProduct(FVector::RightVector, SwipeDirection);
	float RightAngle = FVector::DotProduct(FVector::ForwardVector, SwipeDirection);

	auto Result = CheckForMovementSwipe(UpAngle, RightAngle);
	const bool bFoundSwipe = Result != ENavDirection::Max;
	if (bFoundSwipe)
	{
		TryMove(Result);
	}

	return bFoundSwipe;
}

void ACrsPlayerController::TryMove(ENavDirection::Type Direction)
{
	if (GetCharacter() == nullptr) return;

	auto Char = CastChecked<ACrsCharacter>(GetCharacter());

	// Buffer additional moves
	if (Char->IsMoving())
	{
		Char->QueueMove(Direction);
		return;
	}

	
	auto Point = Char->CurrentPoint;
	if (Point != nullptr && Point->GetNavLink(Direction)->IsTraversable() && Point->GetNavLink(Direction)->LinkedPoint->CanOccupy(Char))
	{
		Char->DestinationPoint = Point->GetNavLink(Direction)->LinkedPoint;
		Char->Move(Direction);
	}
}

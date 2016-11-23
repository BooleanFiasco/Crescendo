// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CrsPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CRESCENDO_API ACrsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACrsPlayerController();

	void TryMove(ENavDirection::Type Direction);
	
protected:
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface
	
	void OnTouchBegin(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchRepeat(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location);

	bool CheckForSwipe(const ETouchIndex::Type FingerIndex, const EInputEvent TouchType);
	ENavDirection::Type CheckForMovementSwipe(const float& UpAngle, const float& RightAngle);

	void CheckForTap(const ETouchIndex::Type FingerIndex);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=TouchInput)
	float SwipeDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=TouchInput)
	float AutoSwipeDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=TouchInput)
	float DiagonalSwipeMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=TouchInput)
	float DiagonalSwipeMax;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=TouchInput)
	float VerticalSwipeAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=TouchInput)
	float HorizontalSwipeAngle;

private:
	FVector TouchLocations[EKeys::NUM_TOUCH_KEYS];
	FVector TouchStartLocations[EKeys::NUM_TOUCH_KEYS];
};

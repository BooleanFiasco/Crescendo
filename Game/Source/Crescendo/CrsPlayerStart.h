// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "CrsNavPointComponent.h"
#include "CrsPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class CRESCENDO_API ACrsPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	// Begin AActor API
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	// End AActor API

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=NavPoint)
	UCrsNavPointComponent* StartPoint;

protected:
#if WITH_EDITOR
	void OnDeferredConstruction();
#endif
	
};

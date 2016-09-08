// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CrsNavPointComponent.h"
#include "CrsTile.generated.h"

UCLASS()
class CRESCENDO_API ACrsTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACrsTile();

	// Begin AActor API
	virtual void BeginPlay() override;
	// End AActor API

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* MeshComponent;
};

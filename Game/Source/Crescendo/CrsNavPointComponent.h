// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CrsNavPointComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRESCENDO_API UCrsNavPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCrsNavPointComponent();

	// Begin USceneComponent API
	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	// End USceneComponent API		

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Linking)
	float LinkDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Linking)
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Linking)
	uint8 ValidDirections[ENavDirection::Max];

	FVector LinkLocations[4];
};

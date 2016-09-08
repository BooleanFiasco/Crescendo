// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsNavPointComponent.h"


// Sets default values for this component's properties
UCrsNavPointComponent::UCrsNavPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	LinkDistance = 120.0f;
	bEnabled = true;
	memset(ValidDirections, 0, sizeof(bool) * ENavDirection::Max);
	memset(LinkLocations, 0, sizeof(FVector) * 4);
}


// Called when the game starts
void UCrsNavPointComponent::BeginPlay()
{
	Super::BeginPlay();

	// Determine orientation
	FVector Loc = GetComponentLocation();
	if (FMath::Abs(FVector::DotProduct(GetUpVector(), FVector::UpVector)) > 0.95f)
	{
		LinkLocations[0] = Loc + FVector::ForwardVector * LinkDistance;
		LinkLocations[1] = Loc - FVector::ForwardVector * LinkDistance;
		LinkLocations[2] = Loc + FVector::RightVector * LinkDistance;
		LinkLocations[3] = Loc - FVector::RightVector * LinkDistance;
	}
	else if (FMath::Abs(FVector::DotProduct(GetUpVector(), FVector::ForwardVector)) > 0.95f)
	{
		LinkLocations[0] = Loc + FVector::UpVector * LinkDistance;
		LinkLocations[1] = Loc - FVector::UpVector * LinkDistance;
		LinkLocations[2] = Loc + FVector::RightVector * LinkDistance;
		LinkLocations[3] = Loc - FVector::RightVector * LinkDistance;
	}
	else if (FMath::Abs(FVector::DotProduct(GetUpVector(), FVector::RightVector)) > 0.95f)
	{
		LinkLocations[0] = Loc + FVector::UpVector * LinkDistance;
		LinkLocations[1] = Loc - FVector::UpVector * LinkDistance;
		LinkLocations[2] = Loc + FVector::ForwardVector * LinkDistance;
		LinkLocations[3] = Loc - FVector::ForwardVector * LinkDistance;
	}
	else
	{
		// ERROR
	}
}


// Called every frame
void UCrsNavPointComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	if (bEnabled)
	{
		DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), GetComponentLocation() + GetUpVector() * 240.0f, 16.0f, FColor::Green, false, 0.0f);
		for (auto Location : LinkLocations)
		{
			DrawDebugSphere(GetWorld(), Location, 16.0f, 16, FColor::Red, false, 0.0f);
		}
	}
}


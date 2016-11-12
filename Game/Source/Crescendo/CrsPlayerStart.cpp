// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsPlayerStart.h"
#include "CrsTile.h"

void ACrsPlayerStart::BeginPlay()
{
	Super::BeginPlay();


}

#if WITH_EDITOR
void ACrsPlayerStart::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACrsPlayerStart::OnDeferredConstruction);
	}
}

void ACrsPlayerStart::OnDeferredConstruction()
{
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
			StartPoint = BestPoint;
		}
		else
		{
			// TODO: This should show a warning, and probably and error if you try to save the map in this state.
		}
	}
}
#endif

// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsTile.h"


// Sets default values
ACrsTile::ACrsTile()
{
 	// Create default components
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACrsTile::BeginPlay()
{
	Super::BeginPlay();
	
	// Break all existing links
	TArray<UCrsNavPointComponent*> NavPoints = GetNavPoints();
	for (auto& Point : NavPoints)
	{
		Point->BreakLinks();
	}

	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACrsTile::OnDeferredRelink);
	}
}

void ACrsTile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (auto Point : GetNavPoints())
	{
		Point->SetTileOwner(this);
	}
}

void ACrsTile::Relink()
{
	OnDeferredRelink();
}

void ACrsTile::OnDeferredRelink()
{
	for (auto& Point : GetNavPoints())
	{
		if (!Point->IsInitialized())
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACrsTile::OnDeferredRelink);
			return;
		}
	}

	// Break all existing links
	for (auto& Point : GetNavPoints())
	{
		Point->BreakLinks();
	}

	// Find other nearby tiles and attempt to link with them
	TArray<FOverlapResult> Results;
	if (GetWorld()->OverlapMultiByObjectType(Results, GetActorLocation(), GetActorQuat(), FCollisionObjectQueryParams::AllObjects, FCollisionShape::MakeBox(FVector(240, 240, 240))))
	{
		for (auto& Result : Results)
		{
			if (Result.GetActor() != nullptr && Result.GetActor()->GetWorld() == GetWorld() && Result.GetActor()->IsA(ACrsTile::StaticClass()))
			{
				AttemptLink(Cast<ACrsTile>(Result.GetActor()));
			}
		}
	}
}

#if WITH_EDITOR
void ACrsTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACrsTile::OnDeferredRelink);
	}
}
#endif

TArray<UCrsNavPointComponent*> ACrsTile::GetNavPoints() const
{
	TArray<UActorComponent*> NavPoints = GetComponentsByClass(UCrsNavPointComponent::StaticClass());
	TArray<UCrsNavPointComponent*> NavPointComps;
	for (auto& Comp : NavPoints)
	{
		auto Point = Cast<UCrsNavPointComponent>(Comp);
		NavPointComps.Add(Point);
	}
	return NavPointComps;
}

void ACrsTile::AttemptLink(ACrsTile* Neighbor)
{
	TArray<UCrsNavPointComponent*> OurNavPoints = GetNavPoints();
	TArray<UCrsNavPointComponent*> TheirNavPoints = Neighbor->GetNavPoints();
	for (auto& OurPoint : OurNavPoints)
	{
		if (!OurPoint->IsEnabled()) continue;

		for (auto& TheirPoint : TheirNavPoints)
		{
			if (!TheirPoint->IsEnabled() || TheirPoint == OurPoint) continue;

			OurPoint->AttemptLink(TheirPoint);
		}
	}
}

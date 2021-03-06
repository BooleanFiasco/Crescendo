// Fill out your copyright notice in the Description page of Project Settings.

#include "Crescendo.h"
#include "CrsNavPointComponent.h"
#include "CrsTile.h"

// Sets default values for this component's properties
UCrsNavPointComponent::UCrsNavPointComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LinkDistance = 120.0f;
	bEnabled = true;
	PointDecalSize = FVector(24, 32, 32);
	LinkDecalSize = FVector(24, 120, 120);
	LinkDecalOffset = 40.0f;

	bInitialized = false;
}

ENavDirection::Type UCrsNavPointComponent::SwipeToNavDirection(ESwipeDirection::Type SwipeDirection)
{
	auto Result = ENavDirection::Max;
	switch (SwipeDirection)
	{
		case ESwipeDirection::DownLeft:
			Result = ENavDirection::Back;
			break;
		case ESwipeDirection::DownRight:
			Result = ENavDirection::Right;
			break;
		case ESwipeDirection::UpLeft:
			Result = ENavDirection::Left;
			break;
		case ESwipeDirection::UpRight:
			Result = ENavDirection::Forward;
			break;
	}

	return Result;
}

#if WITH_EDITOR
void UCrsNavPointComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UCrsNavPointComponent::OnDeferredConstruction);
	}
}

void UCrsNavPointComponent::PreEditChange(UProperty* PropertyThatWillChange)
{
	Super::PreEditChange(PropertyThatWillChange);
}

void UCrsNavPointComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Unlink anyone who's now disabled, and match states for anything set to locked
	for (auto Link : NavLinks)
	{
		if (Link.LinkedPoint != nullptr)
		{
			auto OtherLink = Link.LinkedPoint->GetNavLink(this);
			if (Link.State == ENavLinkState::Disabled)
			{
				if (OtherLink != nullptr) OtherLink->State = ENavLinkState::Available;
				Link.LinkedPoint->UnlinkPoint(this);
				UnlinkPoint(Link.LinkedPoint);
			}
			else
			{
				
				if (OtherLink != nullptr)
				{
					OtherLink->State = Link.State;
				}
			}
			
		}
		else if (Link.State == ENavLinkState::Locked)
		{
			// Don't allow links without points to be set to locked
			Link.State = ENavLinkState::Available;
		}
	}
}

void UCrsNavPointComponent::OnDeferredConstruction()
{
	DetermineLinkLocations();
	CreatePointDecal();

	bInitialized = true;
}
#endif

// Called when the game starts
void UCrsNavPointComponent::BeginPlay()
{
	Super::BeginPlay();

	DetermineLinkLocations();
	bInitialized = true;
}


// Called every frame
void UCrsNavPointComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	/*if (bEnabled)
	{
		//DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), GetComponentLocation() + GetUpVector() * 240.0f, 16.0f, FColor::Green, false, 0.0f);
		for (int i = 0; i < MAX_NAV_LINKS; i++)
		{
			FVector ToLocation = (LinkLocations[i] - GetComponentLocation()).GetSafeNormal();
			FVector DrawSpot = GetComponentLocation() + (ToLocation * 60.0f);
			FColor DrawColor = NavLinks[LinkDirections[i]].LinkedPoint == nullptr ? FColor::Red : FColor::Green;
			DrawDebugSphere(GetWorld(), DrawSpot, 16.0f, 16, DrawColor, false, 0.0f);

			auto Other = NavLinks[LinkDirections[i]].LinkedPoint;
			if (Other != nullptr)
			{
				DrawDebugDirectionalArrow(GetWorld(), DrawSpot + GetUpVector() * 16.0f,
					Other->GetComponentLocation(), 16.0f, FColor::Blue, false, 0.0f);
			}
		}
	}*/
}

void UCrsNavPointComponent::DetermineLinkLocations()
{
	for(auto& NavLink : NavLinks)
	{
		NavLink.Clear();
	}

	// Determine orientation
	FVector Loc = GetComponentLocation();
	if (FMath::Abs(FVector::DotProduct(GetUpVector(), FVector::UpVector)) > 0.95f)
	{
		NavLinks[ENavDirection::Forward].LinkLocation = Loc + FVector::ForwardVector * LinkDistance;
		NavLinks[ENavDirection::Forward].bValid = true;

		NavLinks[ENavDirection::Back].LinkLocation = Loc - FVector::ForwardVector * LinkDistance;
		NavLinks[ENavDirection::Back].bValid = true;

		NavLinks[ENavDirection::Right].LinkLocation = Loc + FVector::RightVector * LinkDistance;
		NavLinks[ENavDirection::Right].bValid = true;

		NavLinks[ENavDirection::Left].LinkLocation = Loc - FVector::RightVector * LinkDistance;
		NavLinks[ENavDirection::Left].bValid = true;
	}
	else if (FMath::Abs(FVector::DotProduct(GetUpVector(), FVector::ForwardVector)) > 0.95f)
	{
		NavLinks[ENavDirection::Up].LinkLocation = Loc + FVector::UpVector * LinkDistance;
		NavLinks[ENavDirection::Up].bValid = true;

		NavLinks[ENavDirection::Down].LinkLocation = Loc - FVector::UpVector * LinkDistance;
		NavLinks[ENavDirection::Down].bValid = true;

		NavLinks[ENavDirection::Right].LinkLocation = Loc + FVector::RightVector * LinkDistance;
		NavLinks[ENavDirection::Right].bValid = true;

		NavLinks[ENavDirection::Left].LinkLocation = Loc - FVector::RightVector * LinkDistance;
		NavLinks[ENavDirection::Left].bValid = true;
	}
	else if (FMath::Abs(FVector::DotProduct(GetUpVector(), FVector::RightVector)) > 0.95f)
	{
		NavLinks[ENavDirection::Up].LinkLocation = Loc + FVector::UpVector * LinkDistance;
		NavLinks[ENavDirection::Up].bValid = true;

		NavLinks[ENavDirection::Down].LinkLocation = Loc - FVector::UpVector * LinkDistance;
		NavLinks[ENavDirection::Down].bValid = true;

		NavLinks[ENavDirection::Forward].LinkLocation = Loc + FVector::ForwardVector * LinkDistance;
		NavLinks[ENavDirection::Forward].bValid = true;

		NavLinks[ENavDirection::Back].LinkLocation = Loc - FVector::ForwardVector * LinkDistance;
		NavLinks[ENavDirection::Back].bValid = true;
	}
	else
	{
		// ERROR
	}
}

bool UCrsNavPointComponent::UnlinkPoint(UCrsNavPointComponent* Other)
{
	// Find the point within our nav link list and remove it
	for (auto NavLink : NavLinks)
	{
		if (NavLink.LinkedPoint == Other)
		{
			NavLink.Unlink();
			return true;
		}
	}

	return false;
}

void UCrsNavPointComponent::BreakLinks()
{
	for (auto NavLink : NavLinks)
	{
		if (!NavLink.bValid || NavLink.LinkedPoint == nullptr) continue;

		NavLink.LinkedPoint->UnlinkPoint(this);
		NavLink.Unlink();
	}
}

bool UCrsNavPointComponent::AttemptLink(UCrsNavPointComponent* Other)
{
	// Test each possible link location
	for (int i = 0; i < ENavDirection::Max; i++)
	{
		if (NavLinks[i].State == ENavLinkState::Disabled || !NavLinks[i].bValid) continue;

		FVector OurLocation = NavLinks[i].LinkLocation;
		for (int j = 0; j < ENavDirection::Max; j++)
		{
			if (Other->NavLinks[j].State == ENavLinkState::Disabled || !Other->NavLinks[j].bValid) continue;

			float DistSq = FVector::DistSquared(OurLocation, Other->NavLinks[j].LinkLocation);
			if (DistSq <= KINDA_SMALL_NUMBER)
			{
				// For locked links we need to make sure our states match up
				if (NavLinks[i].State == ENavLinkState::Locked)
				{
					Other->NavLinks[j].State = ENavLinkState::Locked;
				}

				CreateLink((ENavDirection::Type)i, Other);
				Other->CreateLink((ENavDirection::Type)j, this);
				return true;
			}
		}
	}

	return false;
}

void UCrsNavPointComponent::CreateLink(ENavDirection::Type LocationIndex, UCrsNavPointComponent* Other)
{
	NavLinks[LocationIndex].LinkedPoint = Other;
	CreateLinkDecal(LocationIndex);
}

void UCrsNavPointComponent::CreateLinkDecal(ENavDirection::Type LocationIndex)
{
	if (NavLinks[LocationIndex].LinkedPoint != nullptr)
	{
		FNavLinkDetails* Link = &NavLinks[LocationIndex];
		if (Link->LinkDecal != nullptr)
		{
			Link->LinkDecal->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			Link->LinkDecal->UnregisterComponent();
			Link->LinkDecal = nullptr;
		}

		// Spawn the link decal
		FVector DecalLoc = GetComponentLocation() + ((Link->LinkLocation - GetComponentLocation()) * 0.5f);
		DecalLoc += (DecalLoc - GetComponentLocation()).GetSafeNormal() * LinkDecalOffset * 0.5f;
		FVector Offset = GetUpVector() * 16.0f;

		Link->LinkDecal = NewObject<UDecalComponent>(this, FName(*FString::Printf(TEXT("LinkDecal_%d"), (int)LocationIndex)));
		Link->LinkDecal->SetWorldRotation(GetComponentRotation());
		Link->LinkDecal->SetWorldLocation(DecalLoc + Offset);

		Link->LinkDecal->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		Link->LinkDecal->RegisterComponent();
		Link->LinkDecal->SetDecalMaterial(Link->State == ENavLinkState::Locked ? LinkDecalMaterialLocked : LinkDecalMaterial);
		Link->LinkDecal->SetSortOrder(10);
		Link->LinkDecal->SetWorldScale3D(FVector(1, 1, 1));
		Link->LinkDecal->DecalSize = LinkDecalSize - FVector(0, LinkDecalOffset * 0.5f, LinkDecalOffset * 0.5f);

		float Yaw = 90.0f;
		const bool bRotate = (LocationIndex == ENavDirection::Up || LocationIndex == ENavDirection::Down) && FMath::Abs(FVector::DotProduct(FVector::RightVector, GetUpVector())) > 0.95f;
		if (LocationIndex == ENavDirection::Right || LocationIndex == ENavDirection::Left || bRotate )
		{
			Yaw = 0.0f;
		}
		Link->LinkDecal->SetRelativeRotation(FRotator(90.f, Yaw, 0.0f));
	}
}

void UCrsNavPointComponent::CreatePointDecal()
{
	if (PointDecal != nullptr)
	{
		PointDecal->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		PointDecal->UnregisterComponent();
		PointDecal = nullptr;
	}

	if (bEnabled)
	{
		PointDecal = NewObject<UDecalComponent>(this, FName(TEXT("PointDecal")));
		PointDecal->SetWorldRotation(GetComponentRotation());
		FVector Offset = GetUpVector() * 16.0f;
		PointDecal->SetWorldLocation(GetComponentLocation() + Offset);


		PointDecal->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		PointDecal->RegisterComponent();
		PointDecal->SetDecalMaterial(PointDecalMaterial);
		PointDecal->SetSortOrder(10);
		PointDecal->SetWorldScale3D(FVector(1, 1, 1));
		PointDecal->DecalSize = PointDecalSize;
		PointDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	}
}

FNavLinkDetails* UCrsNavPointComponent::GetNavLink(ENavDirection::Type Direction)
{
	if (Direction == ENavDirection::Max) return nullptr;

	return &NavLinks[Direction];
}

FNavLinkDetails* UCrsNavPointComponent::GetNavLink(UCrsNavPointComponent* Point)
{
	for (int i = 0; i < ENavDirection::Max; i++)
	{
		if (NavLinks[i].bValid && NavLinks[i].LinkedPoint == Point)
		{
			return &NavLinks[i];
		}
	}

	return nullptr;
}

void UCrsNavPointComponent::Occupy(AActor* NewOccupant)
{
	Occupants.AddUnique(NewOccupant);
	for (auto Interactable : InteractableTargets)
	{
		Interactable->Execute_OnActivate(Cast<UObject>(Interactable), this);
	}
	OnOccupiedEvent.Broadcast(NewOccupant);
}

void UCrsNavPointComponent::Leave(AActor* FormerOccupant)
{
	Occupants.Remove(FormerOccupant);
	for (auto Interactable : InteractableTargets)
	{
		Interactable->Execute_OnDeactivate(Cast<UObject>(Interactable), this);
	}
	OnLeftEvent.Broadcast(FormerOccupant);
}

bool UCrsNavPointComponent::CanShare(const AActor& FirstOccupant, const AActor& SecondOccupant) const
{
	return false;
}

bool UCrsNavPointComponent::IsWall() const
{
	return FVector::DotProduct(GetUpVector(), FVector::UpVector) < 0.97f;
}

bool UCrsNavPointComponent::IsLinkCorner(ENavDirection::Type Direction) const
{
	auto Other = NavLinks[Direction].LinkedPoint;
	return IsWall() && Other != nullptr && Other->IsWall() && FVector::DotProduct(GetUpVector(), Other->GetUpVector()) < 0.97f;
}

ENavDirection::Type UCrsNavPointComponent::GetDirectionFromVec(FVector DirVec) const
{
	ENavDirection::Type Result = ENavDirection::Max;
	if (FVector::DotProduct(DirVec, FVector::ForwardVector) > 0.97f)
	{
		Result = ENavDirection::Forward;
	}
	else if (FVector::DotProduct(DirVec, FVector::RightVector) > 0.97f)
	{
		Result = ENavDirection::Right;
	}
	else if (FVector::DotProduct(DirVec, -FVector::ForwardVector) > 0.97f)
	{
		Result = ENavDirection::Back;
	}
	else if (FVector::DotProduct(DirVec, -FVector::RightVector) > 0.97f)
	{
		Result = ENavDirection::Left;
	}

	return Result;
}

bool UCrsNavPointComponent::CanOccupy_Implementation(AActor* NewOccupant) const
{
	return true;
}

void UCrsNavPointComponent::SetLinkState(ENavDirection::Type Direction, ENavLinkState::Type NewState)
{
	if (NavLinks[Direction].State == NewState) return;

	switch (NewState)
	{
		case ENavLinkState::Available:
			if (NavLinks[Direction].State == ENavLinkState::Disabled)
			{
				NavLinks[Direction].State = NewState;
				TileOwner->Relink();
			}
			else
			{
				auto OtherLink = NavLinks[Direction].LinkedPoint->GetNavLink(this);
				OtherLink->State = NewState;
				NavLinks[Direction].State = NewState;
				NavLinks[Direction].LinkedPoint->OnLinkUpdated(OtherLink);
				OnLinkUpdated(&NavLinks[Direction]);
			}
			break;
		case ENavLinkState::Disabled:
			if (NavLinks[Direction].LinkedPoint != nullptr)
			{
				NavLinks[Direction].LinkedPoint->UnlinkPoint(this);
				NavLinks[Direction].LinkedPoint = nullptr;
			}
			NavLinks[Direction].State = NewState;
			break;
		case ENavLinkState::Locked:
			if (NavLinks[Direction].LinkedPoint == nullptr)
			{
				UE_LOG(LogCrs, Warning, TEXT("Tried to lock a nav link that was not linked to a point!"));
			}
			else
			{
				auto OtherLink = NavLinks[Direction].LinkedPoint->GetNavLink(this);
				OtherLink->State = NewState;
				NavLinks[Direction].State = NewState;
				NavLinks[Direction].LinkedPoint->OnLinkUpdated(OtherLink);
				OnLinkUpdated(&NavLinks[Direction]);
			}
			break;
	}
}

void UCrsNavPointComponent::OnLinkUpdated(FNavLinkDetails* UpdatedLink)
{
	// Recreate decals
	int LinkIdx = 0;
	for (LinkIdx = 0; LinkIdx < MAX_NAV_LINKS; LinkIdx++)
	{
		if (&NavLinks[LinkIdx] == UpdatedLink) break;
	}

	if (LinkIdx < MAX_NAV_LINKS)
	{
		CreateLinkDecal((ENavDirection::Type)LinkIdx);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CrsNavPointComponent.generated.h"

#define MAX_NAV_LINKS		4

class UCrsNavPointComponent;

USTRUCT(BlueprintType)
struct CRESCENDO_API FNavLinkDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=LinkDetails)
	bool bEnabled;

	UCrsNavPointComponent* LinkedPoint;
	bool bValid;
	FVector LinkLocation;
	UDecalComponent* LinkDecal;

	FNavLinkDetails() {
		Clear();
	}

	void Clear()
	{
		bEnabled = true;
		LinkedPoint = nullptr;
		bValid = false;
		LinkLocation = FVector::ZeroVector;
		LinkDecal = nullptr;
	}

	void Unlink()
	{
		LinkedPoint = nullptr;
		if (LinkDecal != nullptr)
		{
			LinkDecal->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			LinkDecal->UnregisterComponent();
			LinkDecal = nullptr;
		}
	}
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRESCENDO_API UCrsNavPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCrsNavPointComponent();

	static ENavDirection::Type SwipeToNavDirection(ESwipeDirection::Type SwipeDirection);

	// Begin USceneComponent API
	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
#if WITH_EDITOR
	virtual void OnComponentCreated() override;
#endif
	// End USceneComponent API		

	void OnDeferredConstruction();
	void DetermineLinkLocations();
	bool UnlinkPoint(UCrsNavPointComponent* Other);
	void BreakLinks();
	bool AttemptLink(UCrsNavPointComponent* Other);
	void CreateLink(ENavDirection::Type LocationIndex, UCrsNavPointComponent* Other);
	void CreateLinkDecal(ENavDirection::Type LocationIndex);
	void CreatePointDecal();

	FNavLinkDetails* GetNavLink(ENavDirection::Type Direction);
	FNavLinkDetails* GetNavLink(UCrsNavPointComponent* Point);

	bool IsEnabled() const { return bEnabled; }
	bool IsInitialized() const { return bInitialized; }
	bool IsOccupied() const { return Occupants.Num() > 0; }
	bool IsWall() const;
	
	void Occupy(AActor* NewOccupant);
	void Leave(AActor* FormerOccupant);
	TArray<AActor*> GetOccupants() const { return Occupants; }
	bool CanShare(const AActor& FirstOccupant, const AActor& SecondOccupant) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Linking)
	float LinkDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Linking)
	bool bEnabled;

	UPROPERTY(EditAnywhere, Category = Linking)
	FNavLinkDetails NavLinks[ENavDirection::Max];

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Linking)
	UMaterialInterface* PointDecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Linking)
	FVector PointDecalSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Linking)
	UMaterialInterface* LinkDecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Linking)
	FVector LinkDecalSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Linking)
	float LinkDecalOffset;

	UDecalComponent* PointDecal;

	bool bInitialized;
	TArray<AActor*> Occupants;
};

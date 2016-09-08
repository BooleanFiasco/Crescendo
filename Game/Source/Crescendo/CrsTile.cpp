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
	
}

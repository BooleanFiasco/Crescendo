// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CrsActivatableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCrsActivatableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class CRESCENDO_API ICrsActivatableInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Crescendo)
	void OnActivate(UObject* Activator);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Crescendo)
	void OnDeactivate(UObject* Deactivator);

};

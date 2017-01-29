// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Crescendo.h"

/**
 * 
 */
class CRESCENDO_API ActorComponentPtr
{
public:
	ActorComponentPtr();
	~ActorComponentPtr();

	FName TargetActorName;
	FName TargetComponentName;
};

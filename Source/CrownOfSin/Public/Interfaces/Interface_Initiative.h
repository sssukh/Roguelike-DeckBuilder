// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Initiative.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_Initiative : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_Initiative
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Initiative")
	float GetInitiative();
};

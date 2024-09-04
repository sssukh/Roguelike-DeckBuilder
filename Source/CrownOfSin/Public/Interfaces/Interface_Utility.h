// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Interface_Utility.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_Utility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_Utility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Utility")
	void Initialize(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Utility")
	bool Interact(const FGameplayTagContainer& Tags);
};

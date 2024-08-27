// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_CardGameInstance.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_CardGameInstance : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_CardGameInstance
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	TArray<int32> GetVisitedMapNodesFromInstance();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Game Instance")
	TArray<FDataTableRowHandle> GetDoneStoryEncountersFromInstance();
};

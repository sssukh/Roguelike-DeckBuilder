// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_StatusWidget.generated.h"

class UStatusComponent;
// This class does not need to be modified.
UINTERFACE()
class UInterface_StatusWidget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_StatusWidget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Status Widget")
	UObject* AddStatusIndicator(UStatusComponent* Status, bool bShowImmediately);
};

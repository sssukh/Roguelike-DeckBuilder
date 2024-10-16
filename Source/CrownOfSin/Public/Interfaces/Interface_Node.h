﻿#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Node.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_Node : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_Node
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Node")
	void HoverOverNode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Node")
	void HoverOffNode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Node")
	void ClickNode();
};

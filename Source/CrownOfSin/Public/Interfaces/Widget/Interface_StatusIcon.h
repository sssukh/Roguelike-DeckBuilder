// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Core/CosEnumStruct.h"
#include "UObject/Interface.h"
#include "Interface_StatusIcon.generated.h"

class UStatusComponent;
// This class does not need to be modified.
UINTERFACE()
class UInterface_StatusIcon : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_StatusIcon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Status Icon")
	void InitStatusWidget(UStatusComponent* InStatus, FGameplayTag InStatusBarTag, bool bIsShowImmediately);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Status Icon")
	bool ModifyStatusWidget(int32 NewValue, const FText& TextOverride, bool bRefreshAppearance, const FStatusAppearance& StatusAppearanceIn, bool bCanBeZero);
};

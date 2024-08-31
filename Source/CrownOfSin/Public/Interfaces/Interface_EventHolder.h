// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Interface_EventHolder.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_EventHolder : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_EventHolder
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Event Holder")
	void RunEvent(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags);

	//미구현 미리 코드 생성
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Event Holder")
	float GetPriority();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Event Holder")
	void SetPriority(float InPriority);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Event Holder")
	FName GetFriendlyName();
};

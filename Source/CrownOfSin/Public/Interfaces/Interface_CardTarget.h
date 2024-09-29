// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Interface_CardTarget.generated.h"

class UStatusComponent;
// This class does not need to be modified.
UINTERFACE()
class UInterface_CardTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_CardTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//미구현 재정의 안함 코드만 옮김
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	bool MarkTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	void UnMarkTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	bool AttemptMarkTarget(const FGameplayTagContainer& ValidTargets);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	bool CheckIfValidTarget(const FGameplayTagContainer& ValidTargets);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	int32 AddToStatus(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	AActor* GetPuppet();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	int32 SubtractFromStatus(TSubclassOf<UStatusComponent> InStatusClass, int32 InAmount, bool bIsShowSplash, UObject* InPayLoad);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	bool GetStatusMax(TSubclassOf<UStatusComponent> InStatus, int32& OutMax);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	bool SetStatusMax(TSubclassOf<UStatusComponent> InStatus, int32 NewMax);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	FString GetMinionUniqueID();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Target")
	bool GetStatusValue(TSubclassOf<UStatusComponent> InStatus, int32& OutStatusValue);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Interface_CardPuppet.generated.h"

class AMinionBase;
// This class does not need to be modified.
UINTERFACE()
class UInterface_CardPuppet : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_CardPuppet
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	void InitializePuppet(AMinionBase* InMinion,bool bSpawnedDuringGameplay);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	void AnimatePuppet(FGameplayTag InAnimationTag);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	void SignalAnimEvent(FGameplayTag InAnimationTag, FName InNotify);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	void SignalAnimEnd(FGameplayTag InAnimationTag);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	FVector GetPuppetRelativeCenter();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	bool GetMinionFromPuppet(AMinionBase*& OutMinion);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	UObject* GetPuppetUI(EStatusSlot InSlot);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	void UnmarkPuppet();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	void MarkPuppet();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	void DisappearPuppet(float InSpeedProp);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Puppet")
	void AppearPuppet(float InSpeedProp);
};

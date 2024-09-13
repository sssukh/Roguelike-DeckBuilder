// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Interface.h"
#include "Interface_CardWidget.generated.h"

class ACardBase;
// This class does not need to be modified.
UINTERFACE()
class UInterface_CardWidget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CROWNOFSIN_API IInterface_CardWidget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void UpdateCardWidget(ACardBase* InCardActor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void RequestDesiredTransformUpdate(FWidgetTransform DesiredTransform, bool bEnableMovement);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void ToggleCardGlow(bool bEnable, FColor Color);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void SetCardState(ECardState CardState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void AnimateAutoPlay();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void AnimateCardNotify();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void AnimateCardExhaust(EUMGSequencePlayMode::Type PlayMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void AnimateCardAppear(EUMGSequencePlayMode::Type PlayMode);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void SuggestCardZOrder(int32 ZOrder);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="COS|Interface|Card Widget")
	void ForceCardZOrder(int32 ZOrder);
};

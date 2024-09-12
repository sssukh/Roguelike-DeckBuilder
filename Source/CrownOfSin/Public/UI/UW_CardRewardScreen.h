// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CardRewardScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReturnSelectedCardInRewardScreen,bool,bSkipped,ACardBase*,Card);
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_CardRewardScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_CardRewardScreen(const FObjectInitializer& ObjectInitializer);

public:
	// Delegate
	FOnReturnSelectedCardInRewardScreen OnReturnSelectedCardInRewardScreen;
};

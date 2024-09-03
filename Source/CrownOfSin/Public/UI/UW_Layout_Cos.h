﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Layout_Cos.generated.h"

class UButton;
class UUW_ScreenFade;
class UWidgetSwitcher;
class USizeBox;
class UUW_RewardScreen;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_Layout_Cos : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_Layout_Cos(const FObjectInitializer& ObjectInitializer);

	
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable,Category="UW Layout")
	void SetupDebugMode();
	
	UFUNCTION(BlueprintCallable,Category="UW Layout")
	void UpdateHandAnchorWidgets();

public:
	UFUNCTION(BlueprintCallable, Category="UW Layout Event")
	bool ShouldNodeMapBeBlocked();


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UWidgetSwitcher* DeckListSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	USizeBox* StoryEncounterBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UButton* DebugButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UUW_RewardScreen* WBP_RewardScreen;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UUW_ScreenFade* WBP_ScreenFade;
};

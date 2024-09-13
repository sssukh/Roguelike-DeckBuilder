// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CardRewardScreen.generated.h"

class UButton;
class UImage;
class UUW_CardListCard;
class UTextBlock;
class UHorizontalBox;
class UOverlay;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReturnSelectedCardInRewardScreen, bool, bSkipped, ACardBase*, Card);
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class CROWNOFSIN_API UUW_CardRewardScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_CardRewardScreen(const FObjectInitializer& ObjectInitializer);

	// OnClicked(SkipButton) 블루프린트 구현 필요
	
	UFUNCTION(BlueprintCallable, Category = "Card Reward Scene")
	void UpdateRewardScreen(TArray<ACardBase*>& Cards, const FText& InTitle, bool bAllowSkip);

	UFUNCTION(BlueprintCallable, Category = "Card Reward Scene")
	void ReturnReward(UUW_CardListCard* CardListCard, ACardBase* CardActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Card Reward Scene")
	void ShowCardOptions();
	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene", meta = (BindWidget))
	TObjectPtr<UImage> Image_81;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> RewardBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene", meta = (BindWidget))
	TObjectPtr<UButton> SkipButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene", meta = (BindWidget))
	TObjectPtr<UOverlay> SkipOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene", meta = (BindWidget))
	TObjectPtr<UTextBlock> Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene")
	int32 LoopIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene", meta = (BindWidget))
	float AnimDelay = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene", meta = (BindWidget))
	int32 RewardOptionCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardRewardScene", Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn;
public:
	// Delegate
	UPROPERTY(BlueprintAssignable,BlueprintCallable, Category = "CardRewardScene", meta = (BindWidget))
	FOnReturnSelectedCardInRewardScreen OnReturnSelectedCardInRewardScreen;
};

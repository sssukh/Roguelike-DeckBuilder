#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CardRewardScreen.generated.h"

class ACardBase;
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
UCLASS(Blueprintable, BlueprintType)
class CROWNOFSIN_API UUW_CardRewardScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_CardRewardScreen(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "UW Card Reward Scene")
	void UpdateRewardScreen(const TArray<ACardBase*>& Cards, const FText& InTitle, bool bAllowSkip);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UW Card Reward Scene")
	void ShowCardOptions();

protected:
	UFUNCTION(BlueprintCallable, Category = "UW Card Reward Scene")
	void ReturnReward(UUW_CardListCard* CardListCard, ACardBase* CardActor);

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene", meta = (BindWidget))
	UImage* Image_81;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene", meta = (BindWidget))
	UHorizontalBox* RewardBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene", meta = (BindWidget))
	UButton* SkipButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene", meta = (BindWidget))
	UOverlay* SkipOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene", meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeIn;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene|Config|Setup")
	TSubclassOf<UUW_CardListCard> WBP_CardListCardClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene")
	int32 LoopIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene", meta = (BindWidget))
	float AnimDelay = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card Reward Scene", meta = (BindWidget))
	int32 RewardOptionCount = 3;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UW Card Reward Scene|Delegate")
	FOnReturnSelectedCardInRewardScreen OnReturnSelectedCardInRewardScreen;
};
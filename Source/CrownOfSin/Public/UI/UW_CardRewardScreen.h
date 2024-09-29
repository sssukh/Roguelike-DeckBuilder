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

	/**
	 * 카드 보상 화면을 업데이트하여, 제공된 카드 목록과 화면 타이틀을 설정하고
	 * 스킵 가능 여부에 따라 UI를 조정하는 함수입니다.
	 *
	 * @param AvailableCards 보상으로 제공되는 카드 목록
	 * @param TitleText 화면 상단에 표시할 타이틀 텍스트
	 * @param bIsSkipAllowed 스킵 버튼을 표시할지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "UW Card Reward Scene")
	void UpdateRewardScreen(const TArray<ACardBase*>& AvailableCards, const FText& TitleText, bool bIsSkipAllowed);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UW Card Reward Scene")
	void ShowCardOptions();

protected:
	/**
 	 * 선택한 카드 보상을 반환하고, 해당 화면을 처리하는 함수입니다.
 	 * 카드 UI와 애니메이션을 조정한 후 보상 화면을 닫고, 결과를 콜백으로 전달합니다.
 	 *
 	 * @param SelectedCard 선택된 카드 UI 위젯
 	 * @param SelectedCardActor 선택된 카드 액터
 	 */
	UFUNCTION(BlueprintCallable, Category = "UW Card Reward Scene")
	void ReturnReward(UUW_CardListCard* SelectedCard, ACardBase* SelectedCardActor);

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

private:
	FTimerHandle ReturnCompletionHandle;
	FTimerHandle RewardCompletionHandle;
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UW Card Reward Scene|Delegate")
	FOnReturnSelectedCardInRewardScreen OnReturnSelectedCardInRewardScreen;
};


#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UW_HandSelect.generated.h"

class ACardBase;
class UUW_CardListCard;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmCards, TArray<ACardBase*>&, Cards);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReturnCard,UWidget*, PairedCardWidget);

class UTextBlock;
class UOverlay;
class UHorizontalBox;
class UUW_CardHand;
class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_HandSelect : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_HandSelect(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
public:
	// 초기화 및 카드 생성
	// UW_CardHand의 EnableSelectionMode에서 호출된다. 
	UFUNCTION(BlueprintCallable, Category = "UW HandSelect")
	void Initialize(int32 CardCount, UUW_CardHand* InSourceHand, const FGameplayTagContainer& InValidCardTags);

	// CardListCard의 OnCardClicked에 바인드되며 클릭한 카드가 유효하면 반환한다.
	UFUNCTION(BlueprintImplementableEvent, Category = "UW HandSelect")
	void ReturnCardIfValid(UUW_CardListCard* CardListCard, ACardBase* CardActor);

	// 충분한 만큼의 카드가 선택되면 ConfirmButton을 활성화한다.
	UFUNCTION(BlueprintCallable, Category = "UW HandSelect")
	bool EnableConfirmIfFull();

	// 현재 선택된 모든 카드들을 반환한다.
	UFUNCTION(BlueprintCallable, Category = "UW HandSelect")
	bool GetContainedCards(TArray<ACardBase*>& ContainedCards);

	UFUNCTION(BlueprintCallable, Category = "UW HandSelect")
	void ConfirmButtonOnClicked();
	
	// MemberVariables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CardSelectionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect", meta = (BindWidget))
	TObjectPtr<UTextBlock> CommandText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect", meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect", meta = (BindWidget))
	TObjectPtr<UOverlay> EndTurnOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect", meta = (BindWidget))
	TObjectPtr<UImage> Image_81;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect", meta = (BindWidget))
	TObjectPtr<UUW_CardHand> SourceHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect")
	FGameplayTagContainer ValidCardTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW HandSelect | Config")
	TSubclassOf<UUW_CardListCard> CardListCardClass;
	
	//Delegate
public:
	FOnConfirmCards OnConfirmCards;

	FOnReturnCard OnReturnCard;
};

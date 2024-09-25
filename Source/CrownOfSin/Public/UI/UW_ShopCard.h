#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardSystem/CardBase.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "UW_ShopCard.generated.h"

class UImage;
class UUW_CardListCard;
class UTextBlock;

/**
 * UUW_ShopCard 클래스는 상점에서 판매되는 카드를 나타내는 UI 위젯입니다.
 * 이 위젯은 카드의 정보(이름, 비용 등)를 표시하며, 플레이어가 상점에서 카드를 구매할 수 있는 기능을 제공합니다.
 * 플레이어의 코인 수에 따라 카드 구매 가능 여부가 시각적으로 표시되며, 구매 시 카드가 플레이어 덱에 추가됩니다.
 *
 * 주요 기능:
 * - 카드 정보를 화면에 표시 (코스트, 카드 이미지 등)
 * - 플레이어의 코인 수에 따라 구매 가능 여부 표시
 * - 카드 구매 시 덱에 추가하고 상점에서 제거
 */
UCLASS()
class CROWNOFSIN_API UUW_ShopCard : public UUserWidget, public IInterface_CardWidget
{
	GENERATED_BODY()

public:
	UUW_ShopCard(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	/**
	 * 카드를 구매하려고 시도하는 함수입니다.
	 * 플레이어가 카드의 비용을 지불할 수 있을 경우 카드를 구매하고, 덱에 추가합니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "UW Shop Card")
	void AttemptPurchase(UUW_CardListCard* CardListCard, ACardBase* InCardActor);

	/**
	 * 플레이어의 코인 수에 따라 카드 구매 가능 여부를 업데이트하는 함수입니다.
	 * 코인이 부족할 경우 카드의 텍스트 색상을 변경하여 시각적으로 알립니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "UW Shop Card")
	void UpdateFromCoinAmount(int32 CoinCount);


	/*========================================================================================
	*	IInterface_CardWidget
	=========================================================================================*/
public:
	/**
	 * 카드 위젯을 업데이트하는 함수입니다.
	 * 카드의 데이터와 연결하여 UI를 갱신합니다.
	 */
	virtual void UpdateCardWidget_Implementation(ACardBase* InCardActor) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category = "UW Shop Card|Designer", meta = (BindWidget))
	UImage* CoinImage;

	UPROPERTY(BlueprintReadWrite, Category = "UW Shop Card|Designer", meta = (BindWidget))
	UTextBlock* CostText;

	UPROPERTY(BlueprintReadWrite, Category = "UW Shop Card|Designer", meta = (BindWidget))
	UUW_CardListCard* WBP_CardListCard;

public:
	/** 
	 * 현재 상점에서 판매 중인 카드 객체
	 * 카드의 데이터 및 동작을 제어하는 핵심 데이터입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Shop Card", meta = (ExposeOnSpawn))
	TObjectPtr<ACardBase> CardActor;

	/** 
	 * 카드의 가격 (코스트)
	 * 카드 구매에 필요한 코인 수입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Shop Card")
	int32 Cost;

	/** 
	 * 플레이어가 카드를 구매할 수 있을 때 사용할 텍스트 색상
	 * 코인이 충분한 경우 표시되는 색상입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Shop Card")
	FSlateColor CanAffordColor;

	/** 
	 * 플레이어가 카드를 구매할 수 없을 때 사용할 텍스트 색상
	 * 코인이 부족한 경우 표시되는 색상입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Shop Card")
	FSlateColor CannotAffordColor = FSlateColor(FLinearColor(1.0f, 0.130208f, 0.130208f, 0.672f));
};

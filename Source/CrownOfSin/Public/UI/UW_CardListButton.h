#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_EventHolder.h"
#include "UW_CardListButton.generated.h"

class UWidgetSwitcher;
class UUW_CardList;
class UUW_CardListCard;
class UUW_ToolTip;
class UPileComponent;
class UOverlay;
class UButton;
class UBorder;
class UImage;
class UTextBlock;


/**
 * UUW_CardListButton 클래스는 카드 리스트 UI와 상호작용하는 버튼을 제공하는 위젯입니다.
 * 이 버튼은 플레이어가 카드를 관리하거나 확인할 수 있는 카드 리스트 UI를 열고 닫는 역할을 합니다.
 * 버튼을 클릭하면 해당 카드 리스트가 표시되며, 카드 수량과 상태를 동적으로 업데이트합니다.
 * 
 * 주요 기능:
 * - 카드 리스트 버튼을 클릭하여 카드 리스트를 표시/숨김 처리
 * - PileComponent와 연동하여 카드 수량을 실시간으로 업데이트
 * - 카드 리스트가 표시되면, 외부 클릭 시 리스트가 닫히도록 처리
 * - 버튼에 툴팁을 추가하여 추가적인 설명 제공
 */
UCLASS()
class CROWNOFSIN_API UUW_CardListButton : public UUserWidget, public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	UUW_CardListButton(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

public:
	// PileReference로부터 카드 수량을 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category="UW Card List Button")
	void UpdateAmountFromPileRef();

	// 직접 카드 수량을 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category="UW Card List Button")
	void UpdateAmountDirectly(int32 NewAmount);

protected:
	// 외부 클릭 시 리스트를 숨기는 함수
	UFUNCTION()
	void HideOnOutsideClicked();

	// 카드 리스트 버튼 클릭 시 호출되는 함수
	UFUNCTION()
	void OnClicked_CardListButton();


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button|Designer", meta=(BindWidget))
	UOverlay* TextOverlay;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button|Designer", meta=(BindWidget))
	UTextBlock* Text;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button|Designer", meta=(BindWidget))
	UTextBlock* AmountText;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button|Designer", meta=(BindWidget))
	UImage* DeckIconImage;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button|Designer", meta=(BindWidget))
	UBorder* DeckButtonBorder;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button|Designer", meta=(BindWidget))
	UButton* CardListButton;

	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Add;

public:
	// 카드 리스트 툴팁 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List Button|Config|Setup")
	TSubclassOf<UUW_ToolTip> WBP_ToolTipClass;

	// 카드 리스트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List Button|Config|Setup")
	TSubclassOf<UUW_CardList> WBP_CardListClass;

	// 버튼이 좌우 반전되어 표시될지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List Button", meta=(ExposeOnSpawn="true"))
	bool bFlip;

	// 기본 아이콘의 좌측 패딩
	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button")
	float DefaultIconLeftPadding = 7.0f;

	// 버튼 텍스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List Button")
	FText ButtonText;

	// 덱 아이콘 텍스처
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List Button")
	UTexture2D* DeckIcon;

	// 덱 아이콘의 색상
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List Button", meta=(ExposeOnSpawn="true"))
	FLinearColor DeckTint;

	// 참조할 Pile 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List Button", meta=(ExposeOnSpawn="true"))
	TSubclassOf<UPileComponent> PileClass;

	// 카드 리스트와 연동된 Pile 컴포넌트 참조
	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button")
	UPileComponent* PileReference;

	// 생성된 카드 리스트 위젯
	UPROPERTY(BlueprintReadWrite, Category="UW Card List Button")
	UUW_CardList* CardList;

	// 카드 리스트가 포함된 WidgetSwitcher 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card List Button")
	UWidgetSwitcher* WidgetSwitcherReference;
};

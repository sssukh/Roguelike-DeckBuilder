#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_Utility.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "UW_CardListCard.generated.h"

class UUW_CardVisual;
class UUW_ToolTipList;
class UButton;
class USizeBox;
class ACardBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCardClicked, UUW_CardListCard*, CardListCard, ACardBase*, CardActor);

/**
 * UUW_CardListCard 클래스는 카드 목록에서 개별 카드를 표시하는 위젯입니다.
 * 
 * 주요 기능:
 * - 카드를 시각적으로 표현하고, 해당 카드에 대한 클릭, 호버 등의 이벤트를 처리
 * - 카드 위젯의 시각적 요소를 동적으로 설정하고, 카드 관련 툴팁을 표시
 * - 게임플레이에서 카드와 상호작용할 수 있도록 다양한 인터페이스를 구현
 *
 * 멤버 변수:
 * - 카드 시각적 요소(크기, 애니메이션, 버튼 등)를 포함한 다양한 UI 컴포넌트를 관리
 * - 카드 액터와 연동하여 카드의 시각적 상태를 업데이트
 * - 카드 클릭 이벤트 등 사용자 상호작용을 처리하는 델리게이트 제공
 */
UCLASS(BlueprintType, Blueprintable)
class CROWNOFSIN_API UUW_CardListCard : public UUserWidget, public IInterface_Utility, public IInterface_CardWidget
{
	GENERATED_BODY()

public:
	UUW_CardListCard(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;
	
	virtual void NativeConstruct() override;

	/**
	 * 카드 액터를 기반으로 카드 시각적 요소 위젯을 설정하는 함수입니다.
	 * - 기존의 카드 시각적 요소 위젯을 제거하고, 새로 생성된 위젯을 카드 시각적 요소 박스에 추가합니다.
	 *
	 * @param InCardActor 시각적으로 표현할 카드 액터
	 * @return 새롭게 설정된 카드 시각적 요소 위젯
	 */	
	UFUNCTION(BlueprintCallable, Category="UW Card List Card Event")
	UUserWidget* SetCardVisualWidget(ACardBase* InCardActor);

protected:
	UFUNCTION()
	void OnHovered_HoverButton();

	UFUNCTION()
	void OnClicked_HoverButton();

	UFUNCTION()
	void OnUnhovered_HoverButton();


	/*========================================================================================
	*	IInterface_Utility
	=========================================================================================*/
public:
	virtual void Initialize_Implementation(const FGameplayTagContainer& Tags) override;

	/*========================================================================================
   *	IInterface_CardWidget
   =========================================================================================*/
public:
	virtual void UpdateCardWidget_Implementation(ACardBase* InCardActor) override;


	/*========================================================================================
	 *	Field Members
	 =========================================================================================*/
public:
	/** 카드 시각적 요소를 표시하는 크기 박스 (카드의 시각적 요소를 담는 컨테이너) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card|Designer", meta = (BindWidget))
	USizeBox* CardVisualBox;

	/** 카드에 대한 호버 및 클릭 이벤트 처리를 위한 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card|Designer", meta = (BindWidget))
	UButton* HoverButton;

	/** 카드의 툴팁 정보를 표시하는 툴팁 위젯 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card|Designer", meta = (BindWidget))
	UUW_ToolTipList* WBP_TooltipList;

	/** 카드가 호버될 때 실행되는 애니메이션 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card|Designer", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Hover;

	/** 카드가 선택될 때 실행되는 애니메이션 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card|Designer", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Select;

	/** 카드가 슬롯에서 제거될 때 실행되는 페이드아웃 애니메이션 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card|Designer", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOut;

public:
	/** 카드 시각적 요소를 표현하는 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card|Config|SetUp")
	TSubclassOf<UUW_CardVisual> CardVisualClass;

	/** 호버 애니메이션이 블록(비활성화)될지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card")
	bool bBlockHoverAnim;

	/** 현재 위젯과 연동된 카드 액터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card", meta = (ExposeOnSpawn))
	ACardBase* CardActor;

	/** 이 카드와 연결된 다른 위젯 (예: 선택된 카드와 관련된 위젯) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card")
	UWidget* PairedWidget;

	/** 카드의 시각적 요소를 표현하는 위젯 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UW Card List Card")
	UUserWidget* CardVisual;

public:
	/** 카드 클릭 이벤트를 처리하는 델리게이트 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Card List Card|Delegate")
	FOnCardClicked OnCardClicked;
};
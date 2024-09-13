#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Shop.generated.h"

class UButton;
class UPileComponent;
class UUniformGridPanel;
class UUW_ShopCard;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPileRefSet, UPileComponent*, InPile);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutsideClicked);

/**
 * UUW_Shop 클래스는 게임 내 상점 UI 위젯을 관리합니다.
 * 상점 카드의 업데이트, 버튼 클릭 처리, 카드의 가시성 제어 등의 기능을 제공합니다.
 */
UCLASS()
class CROWNOFSIN_API UUW_Shop : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_Shop(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	/**
	 * 상점 카드 패널을 초기화한 후, 현재 상점의 카드들을 생성하고 그리드에 배치합니다.
	 * 카드의 구매 가능 여부를 업데이트하고, 상점을 화면에 표시합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Shop Evnet")
	void UpdateShopCards();

	/*플레이어의 코인 수에 따라 상점 카드의 구매 가능 여부를 업데이트합니다.*/
	UFUNCTION(BlueprintCallable, Category="UW Shop Evnet")
	void UpdateCardsAvailability();

protected:
	/*Exit 버튼 클릭 시 상점을 닫는 함수*/
	UFUNCTION()
	void OnClicked_ExitButton();

	/*Outside 버튼 클릭 시 상점을 닫는 함수*/
	UFUNCTION()
	void OnClicked_OutsideButton();


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	/** 
	 * 상점 카드들이 배치되는 그리드 패널
	 * 상점 카드들이 그리드 형식으로 배열되는 UI 구성 요소
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Shop|Designer", meta=(BindWidget))
	UUniformGridPanel* CardPanel;

	/** 
	 * 외부 영역을 클릭했을 때 상점을 닫기 위한 버튼
	 * 상점 외부의 빈 공간을 클릭하면 상점이 닫힙니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Shop|Designer", meta=(BindWidget))
	UButton* OutsideButton;

	/** 
	 * 상점 종료 버튼
	 * 이 버튼을 클릭하면 상점이 닫힙니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Shop|Designer", meta=(BindWidget))
	UButton* ExitButton;

public:
	/**
	 * 상점 카드 위젯 클래스
	 * 이 클래스는 상점에서 카드 UI를 생성하는 데 사용됩니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UW Shop|Config|Setup")
	TSubclassOf<UUW_ShopCard> WBP_ShopCardClass;

	/** 
	 * 디버그 모드를 표시하기 위한 플래그
	 * 디버그 모드에서는 상점 카드가 임의로 생성됩니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Shop")
	bool bDebugDisplay = false;

	/** 
	 * 이전에 기록된 코인 수
	 * 상점 카드의 구매 가능 여부를 결정하는 데 사용됩니다.
	 */
	UPROPERTY(BlueprintReadWrite, Category="UW Shop")
	int32 PreviousCoinCount = -1;

	/** 
	 * 상점 카드 그리드의 열 길이
	 * 상점 카드가 몇 개의 열로 배치될지를 결정합니다.
	 */
	UPROPERTY(BlueprintReadWrite, Category="UW Shop")
	int32 RowLength = 4;

public:
	/**
	 * 카드 더미(Pile)가 설정될 때 호출되는 델리게이트
	 * 상점과 관련된 Pile 설정 시 바인딩된 함수가 호출됩니다.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Shop|Delegate")
	FOnPileRefSet OnPileRefSet;

	/**
	 * 외부 버튼이 클릭되었을 때 호출되는 델리게이트
	 * 상점 외부를 클릭하면 이 델리게이트가 실행됩니다.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Shop|Delegate")
	FOnOutsideClicked OnOutsideClicked;
};

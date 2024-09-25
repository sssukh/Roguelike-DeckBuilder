#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Shop.generated.h"

class UButton;
class UPileComponent;
class UUniformGridPanel;
class UUW_ShopCard;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPileRefSet_Shop, UPileComponent*, InPile);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutsideClicked_Shop);


/**
 * UUW_Shop 클래스는 게임 내 상점 UI를 관리하는 위젯입니다.
 * 이 클래스는 플레이어가 상점에서 카드를 구매하거나 상점을 닫을 수 있도록 합니다.
 * 상점의 카드들은 그리드 패널에 배치되며, 플레이어의 코인 상태에 따라 구매 가능 여부가 업데이트됩니다.
 * 또한, 상점 외부를 클릭하거나 'Exit' 버튼을 클릭하면 상점을 닫을 수 있습니다.
 * 
 * 주요 기능:
 * - 상점 카드 목록을 표시하고 그리드 형식으로 배열
 * - 플레이어의 코인 수에 따라 카드의 구매 가능 여부 업데이트
 * - 'Exit' 버튼이나 상점 외부 클릭 시 상점 닫기
 * - 상점이 표시되면 실시간으로 카드 구매 가능 여부를 업데이트
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
	UPROPERTY(BlueprintReadWrite, Category="UW Shop|Designer", meta=(BindWidget))
	UUniformGridPanel* CardPanel;

	UPROPERTY(BlueprintReadWrite, Category="UW Shop|Designer", meta=(BindWidget))
	UButton* OutsideButton;

	UPROPERTY(BlueprintReadWrite, Category="UW Shop|Designer", meta=(BindWidget))
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UW Shop")
	int32 RowLength = 4;

public:
	/**
	 * 카드 더미(Pile)가 설정될 때 호출되는 델리게이트
	 * 상점과 관련된 Pile 설정 시 바인딩된 함수가 호출됩니다.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Shop|Delegate")
	FOnPileRefSet_Shop OnPileRefSet;

	/**
	 * 외부 버튼이 클릭되었을 때 호출되는 델리게이트
	 * 상점 외부를 클릭하면 이 델리게이트가 실행됩니다.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Shop|Delegate")
	FOnOutsideClicked_Shop OnOutsideClicked;
};

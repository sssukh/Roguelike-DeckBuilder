#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "UW_Layout_Cos.generated.h"

class UUW_MessageNotifyBox;
class UUW_DispatcherHubDebug;
class UUW_InGameMenu;
class UUW_GameOver;
class UUW_PlayerStatusIndicator;
class UOverlay;
class UUW_Anchor;
class UUW_CardListButton;
class UUW_StatusBar;
class UUW_CardRewardScreen;
class UUW_CardSelectorList;
class UUW_ArtifactRewardScreen;
class UUW_CardHand;
class UUW_Shop;
class UButton;
class UUW_ScreenFade;
class UWidgetSwitcher;
class USizeBox;
class UUW_RewardScreen;

/*
 * 
 * Crown of Sin 게임의 레이아웃을 담당하는 UUserWidget 기반 클래스입니다.
 * 이 클래스는 게임 내 다양한 위젯 요소를 관리하고, 특정 이벤트 및 상호작용을 처리합니다.
 * 또한 여러 인터페이스를 구현하여 스토리 이벤트, 상태 위젯 관리, 그리고 사용자 이벤트 처리를 수행합니다.
 *
 * 주요 기능:
 * - 디버그 모드 설정 및 디버그 버튼 관리
 * - 손 패 카드 앵커 위젯 관리 및 업데이트
 * - 각종 더미, 폐기, 소모 카드 리스트와 관련된 위젯 요소의 설정 및 관리
 * - 게임의 끝 턴 버튼 처리 및 턴 종료 이벤트 처리
 * - 전투 위젯 숨기기, 메뉴 토글, 핸드 앵커 위젯 업데이트 기능 제공
 * - 다양한 스토리 이벤트 처리, 전투 상황에 맞춰 UI 업데이트
 * - IInterface_StoryEncounter, IInterface_StatusWidget, IInterface_EventHolder 인터페이스 구현
 * 
 * 인터페이스 구현:
 * 1. IInterface_StoryEncounter: 스토리 이벤트 초기화 및 처리
 * 2. IInterface_StatusWidget: 상태 아이콘을 UI에 추가하거나 관리
 * 3. IInterface_EventHolder: 다양한 게임 이벤트 처리 및 반응
 * 
 * 필드 멤버:
 * - 여러 위젯 요소 (버튼, 오버레이, 위젯 스위처 등)를 UPROPERTY로 선언하여 블루프린트에서 쉽게 접근 가능
 * - TMap 자료구조를 사용해 특정 카드 더미의 위젯을 관리
 * - 게임의 플레이어 턴 여부를 관리하는 bool 변수 (bPlayerTurn)
 *
 * 주의 사항:
 * - 위젯과 UI 컴포넌트는 게임 실행 중에 동적으로 변경될 수 있으므로 항상 유효성 검사를 통해 관리해야 합니다.
 * - 디버그 모드 설정 및 핸들링은 블루프린트 또는 코드 내에서 활성화될 수 있으며, 게임의 UI 요소 가시성에 영향을 미칩니다.
 */
UCLASS()
class CROWNOFSIN_API UUW_Layout_Cos : public UUserWidget, public IInterface_StoryEncounter, public IInterface_StatusWidget, public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	UUW_Layout_Cos(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	/**
	 * @brief 디버그 모드를 설정하고 디버그 버튼의 가시성을 변경하는 함수.
	 * 게임 인스턴스에서 디버그 모드를 확인한 후, 해당 모드에 따라 버튼을 보여주거나 숨깁니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void SetupDebugMode();

	/**
	 * @brief 카드 리스트 버튼 위젯을 탐색하여 카드 더미에 해당하는 위젯을 PileWidgets 맵에 추가하는 함수.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void SetupPileWidgetMap();

	/**
	 * @brief 핸드에 배치될 카드 관련 앵커 위젯을 업데이트하는 함수.
	 * 특정 게임 이벤트와 관련된 위젯들을 AnchorWidgetMap에 추가하고 이를 WBP_CardHand 위젯에 설정합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void UpdateHandAnchorWidgets();

public:
	/**
	 * @brief 노드 맵이 차단되어야 하는지 여부를 확인하는 함수.
	 * 특정 UI 위젯이 보이거나 조건을 충족하면 노드 맵이 차단됩니다.
	 * @return 차단 여부를 나타내는 bool 값 (true: 차단됨, false: 차단되지 않음)
	 */
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	bool ShouldNodeMapBeBlocked();

	/**
	 * @brief 전투 관련 위젯들을 숨기는 함수.
	 * 전투 중에 보이는 카드 리스트 버튼 및 기타 UI 요소들을 숨깁니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void HideCombatWidgets();

	/**
	 * @brief 인게임 메뉴의 가시성을 토글하는 함수.
	 * 현재 가시성에 따라 메뉴를 보여주거나 숨깁니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void ToggleInGameMenu();

	/**
	 * @brief 특정 PileTag에 해당하는 카드 더미 위젯의 수량을 업데이트하는 함수.
	 * @param PileTag 카드 더미를 식별하는 태그
	 * @param NewAmount 새로운 수량 값
	 */
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void UpdatePileWidgetAmount(FGameplayTag PileTag, int32 NewAmount);

	/**
	 * @brief 화면에 로그 메시지를 표시하는 함수.
	 * @param Message 화면에 표시할 메시지
	 * @param Color 메시지의 텍스트 색상
	 */
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void DisplayScreenLogMessage(const FText& Message, const FLinearColor& Color);

public:
	/**
	 * @brief 턴 종료 버튼이 활성화될 수 있는지 여부를 반환하는 함수.
	 * @return 버튼 활성화 여부 (true: 활성화됨, false: 비활성화됨)
	 */
	UFUNCTION(BlueprintPure, Category="UW Layout")
	bool GetEndTurnButtonIsEnabled();

	/**
	 * @brief 화폐 관련 UI 박스의 가시성을 반환하는 함수.
	 * @return 가시성 (Visible 또는 Collapsed)
	 */
	UFUNCTION(BlueprintPure, Category="UW Layout")
	ESlateVisibility GetCurrencyBoxVisibility();

protected:
	/**
	 * @brief 플레이어의 턴 종료 버튼이 클릭되었을 때 호출되는 함수.
	 * 턴을 종료하고 관련 이벤트를 발생시킵니다.
	 */
	UFUNCTION()
	void OnClicked_EndTurnButton();

	/**
	 * @brief 디버그 버튼 클릭 시 디버그 UI와 관련된 여러 위젯의 가시성을 토글하는 함수.
	 * 디버그 UI와 카드 리스트 버튼의 가시성을 현재 상태에 따라 전환합니다.
	 */
	UFUNCTION()
	void OnClicked_DebugButton();

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	/*========================================================================================
	*	Iinterface_StoryEncounter
	=========================================================================================*/
public:
	virtual void InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen) override;


	/*========================================================================================
	*	Iinterface_StoryEncounter
	=========================================================================================*/
public:
	virtual UObject* AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UWidgetSwitcher* DeckListSwitcher;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UOverlay* EndTurnOverlay;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UButton* EndTurnButton;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UButton* DebugButton;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	USizeBox* StoryEncounterBox;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_GameOver* WBP_GameOver;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_MessageNotifyBox* WBP_MessageNotifyBox;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_RewardScreen* WBP_RewardScreen;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_ScreenFade* WBP_ScreenFade;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_PlayerStatusIndicator* WBP_StatIndicator_PlayerMana;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_Shop* WBP_Shop;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardHand* WBP_CardHand;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_DispatcherHubDebug* WBP_DispatcherHubDebug;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_ArtifactRewardScreen* WBP_ArtifactReward;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardSelectorList* WBP_CardSelectorList;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardRewardScreen* WBP_CardRewardScreen;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_StatusBar* WBP_ArtifactBar;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_StatusBar* WBP_CurrencyBar;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardListButton* WBP_CardListButton_Deck;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardListButton* WBP_CardListButton_Destroy;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardListButton* WBP_CardListButton_Void;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardListButton* WBP_CardListButton_Draw;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardListButton* WBP_CardListButton_Discard;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_CardListButton* WBP_CardListButton_Exhaust;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_Anchor* WBP_AnchorDeepDraw;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_Anchor* WBP_AnchorPlay;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos|Designer", meta=(BindWidget))
	UUW_InGameMenu* WBP_InGameMenu;

public:
	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos")
	bool bPlayerTurn = false;

	UPROPERTY(BlueprintReadWrite, Category="UW Layout Cos")
	TMap<FGameplayTag, UUW_CardListButton*> PileWidgets;
};

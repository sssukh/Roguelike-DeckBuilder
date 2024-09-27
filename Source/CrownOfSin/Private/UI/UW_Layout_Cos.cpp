#include "UI/UW_Layout_Cos.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Core/CardGameSpawner.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/StatusComponent.h"
#include "UI/UW_CardHand.h"
#include "UI/UW_RewardScreen.h"
#include "UI/UW_ScreenFade.h"
#include "Utilities/CosLog.h"
#include "UI/UW_StatusBar.h"
#include "Utilities/CosGameplayTags.h"
#include "UI/UW_CardListButton.h"
#include "UI/UW_Anchor.h"
#include "UI/UW_DispatcherHubDebug.h"
#include "UI/UW_GameOver.h"
#include "UI/UW_InGameMenu.h"
#include "UI/UW_MessageNotifyBox.h"
#include "UI/UW_PlayerStatusIndicator.h"


UUW_Layout_Cos::UUW_Layout_Cos(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              DeckListSwitcher(nullptr), EndTurnOverlay(nullptr), EndTurnButton(nullptr), DebugButton(nullptr),
                                                                              StoryEncounterBox(nullptr), WBP_GameOver(nullptr),
                                                                              WBP_MessageNotifyBox(nullptr),
                                                                              WBP_RewardScreen(nullptr),
                                                                              WBP_ScreenFade(nullptr), WBP_StatIndicator_PlayerMana(nullptr),
                                                                              WBP_Shop(nullptr), WBP_CardHand(nullptr), WBP_DispatcherHubDebug(nullptr), WBP_ArtifactReward(nullptr),
                                                                              WBP_CardSelectorList(nullptr),
                                                                              WBP_CardRewardScreen(nullptr),
                                                                              WBP_ArtifactBar(nullptr), WBP_CurrencyBar(nullptr),
                                                                              WBP_CardListButton_Deck(nullptr), WBP_CardListButton_Destroy(nullptr), WBP_CardListButton_Void(nullptr),
                                                                              WBP_CardListButton_Draw(nullptr),
                                                                              WBP_CardListButton_Discard(nullptr),
                                                                              WBP_CardListButton_Exhaust(nullptr), WBP_AnchorDeepDraw(nullptr), WBP_AnchorPlay(nullptr), WBP_InGameMenu(nullptr)
{
}

void UUW_Layout_Cos::NativeConstruct()
{
	Super::NativeConstruct();

	// 화면 페이드 애니메이션을 반대로 재생
	PlayAnimationReverse(WBP_ScreenFade->Occlude, 0.5f);

	// 디버그 모드 설정
	SetupDebugMode();

	// 핸드 앵커 위젯 설정
	UpdateHandAnchorWidgets();

	// 카드 더미 위젯 맵 설정
	SetupPileWidgetMap();

	// 카드 게임 스포너를 가져와 유효성 검사를 수행
	ACardGameSpawner* CardGameSpawner = Cast<ACardGameSpawner>(UGameplayStatics::GetActorOfClass(this, ACardGameSpawner::StaticClass()));

	// 이벤트 태그 생성 및 바인딩
	FGameplayTagContainer EventTags;
	if (IsValid(CardGameSpawner))
	{
		// 카드 게임 스포너가 있을 때 추가되는 이벤트들
		EventTags.AddTag(CosGameTags::Event_Action_GameOver);
		EventTags.AddTag(CosGameTags::Event_Action_NewPlayerTurn);
		EventTags.AddTag(CosGameTags::Event_Action_WinGame);
		EventTags.AddTag(CosGameTags::Event_DeckSetupDone);
		EventTags.AddTag(CosGameTags::Event_Victory);

		// 이벤트 허브에 이벤트 바인딩
		UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this, EventTags);

		// 카드 플레이어 찾기 및 유효성 검사
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
		COS_IF_CHECK_VOID(CardPlayer, TEXT("CardPlayer를 월드에서 찾지 못했습니다."));

		// 플레이어 상태 표시 위젯들 초기화
		TArray<UUserWidget*> StatusWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, StatusWidgets, UUW_PlayerStatusIndicator::StaticClass(), false);

		for (UUserWidget* Widget : StatusWidgets)
		{
			UUW_PlayerStatusIndicator* StatusIndicator = Cast<UUW_PlayerStatusIndicator>(Widget);
			if (!StatusIndicator) continue;

			// 카드 플레이어의 상태 컴포넌트 가져오기
			UActorComponent* StatusComponent = CardPlayer->GetComponentByClass(StatusIndicator->StatusClass);
			if (IsValid(StatusComponent))
			{
				UStatusComponent* PlayerStatus = Cast<UStatusComponent>(StatusComponent);
				IInterface_StatusIcon::Execute_InitStatusWidget(StatusIndicator, PlayerStatus, FGameplayTag(), false);
			}
		}
	}
	else
	{
		// 카드 스포너가 없을 때는 특정 이벤트만 바인딩
		EventTags.AddTag(CosGameTags::Event_Action_GameOver);
		EventTags.AddTag(CosGameTags::Event_Action_WinGame);
		EventTags.AddTag(CosGameTags::Event_DeckSetupDone);

		// 이벤트 허브에 이벤트 바인딩
		UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(this, EventTags);

		// 카드 스포너가 없을 때 전투 관련 위젯 숨기기 및 기본 이벤트 바인딩
		HideCombatWidgets();
	}


	// 버튼 클릭 이벤트 처리
	EndTurnButton->OnClicked.AddDynamic(this, &UUW_Layout_Cos::OnClicked_EndTurnButton);
	DebugButton->OnClicked.AddDynamic(this, &UUW_Layout_Cos::OnClicked_DebugButton);
}

void UUW_Layout_Cos::SetupDebugMode()
{
	// GameInstance 가져오기 및 유효성 검사
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!IsValid(CardGameInstance))
	{
		// GameInstance가 유효하지 않으면 디버그 버튼을 숨김
		DebugButton->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 디버그 모드 여부 확인
	bool bIsDebugMode = IInterface_CardGameInstance::Execute_IsDebugMode(CardGameInstance);

	// 디버그 모드에 따라 버튼 가시성 설정
	ESlateVisibility NewVisibility = bIsDebugMode ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	DebugButton->SetVisibility(NewVisibility);
}

void UUW_Layout_Cos::SetupPileWidgetMap()
{
	// 모든 카드 리스트 버튼 위젯을 탐색
	TArray<UUserWidget*> FoundCardListButtons;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundCardListButtons, UUW_CardListButton::StaticClass(), false);

	// 탐색된 위젯들에 대해 더미 맵에 추가
	for (UUserWidget* FoundCardListButton : FoundCardListButtons)
	{
		UUW_CardListButton* CardListButton = Cast<UUW_CardListButton>(FoundCardListButton);
		if (!CardListButton || !IsValid(CardListButton->PileClass))
		{
			// 카드 리스트 버튼 또는 PileClass가 유효하지 않으면 다음으로 넘어감
			continue;
		}

		// PileComponent 기본 객체 가져오기
		UPileComponent* PileCDO = CardListButton->PileClass->GetDefaultObject<UPileComponent>();
		if (!IsValid(PileCDO))
		{
			// PileCDO가 유효하지 않으면 넘어감
			continue;
		}

		// 카드 더미 위젯을 PileWidgets 맵에 추가
		PileWidgets.Add(PileCDO->PileTag, CardListButton);
	}
}

void UUW_Layout_Cos::UpdateHandAnchorWidgets()
{
	TMap<FGameplayTag, UUserWidget*> AnchorWidgetMap;
	AnchorWidgetMap.Add(CosGameTags::Pile_Deck, WBP_CardListButton_Deck);
	AnchorWidgetMap.Add(CosGameTags::Pile_Draw, WBP_CardListButton_Draw);
	AnchorWidgetMap.Add(CosGameTags::Pile_Discard, WBP_CardListButton_Discard);
	AnchorWidgetMap.Add(CosGameTags::Pile_Exhaust, WBP_CardListButton_Exhaust);
	AnchorWidgetMap.Add(CosGameTags::Event_Action_Draw, WBP_AnchorDeepDraw);
	AnchorWidgetMap.Add(CosGameTags::Event_Card_PrePlay, WBP_AnchorPlay);
	WBP_CardHand->AddAnchorWidgets(AnchorWidgetMap);
}

bool UUW_Layout_Cos::ShouldNodeMapBeBlocked()
{
	return WBP_RewardScreen->GetVisibility() == ESlateVisibility::Visible // 보상 화면이 표시되거나  
		|| StoryEncounterBox->GetChildrenCount() > 0 //스토리 엔카운터가 있거나
		|| DeckListSwitcher->IsVisible(); //덱 리스트 스위처가 보이는 경우 노드 맵을 차단
}

void UUW_Layout_Cos::HideCombatWidgets()
{
	WBP_CardListButton_Discard->SetVisibility(ESlateVisibility::Hidden);
	WBP_CardListButton_Draw->SetVisibility(ESlateVisibility::Hidden);
	WBP_CardListButton_Exhaust->SetVisibility(ESlateVisibility::Hidden);
	EndTurnOverlay->SetVisibility(ESlateVisibility::Hidden);
	WBP_StatIndicator_PlayerMana->SetVisibility(ESlateVisibility::Hidden);
}

void UUW_Layout_Cos::ToggleInGameMenu()
{
	// 인게임 메뉴의 현재 가시성을 토대로 토글
	ESlateVisibility CurrentVisibility = WBP_InGameMenu->GetVisibility();

	ESlateVisibility NewVisibility = ESlateVisibility::Collapsed;
	if (CurrentVisibility == ESlateVisibility::Visible) NewVisibility = ESlateVisibility::Hidden;
	else if (CurrentVisibility == ESlateVisibility::Collapsed) NewVisibility = ESlateVisibility::Visible;
	else if (CurrentVisibility == ESlateVisibility::Hidden) NewVisibility = ESlateVisibility::Visible;
	else if (CurrentVisibility == ESlateVisibility::HitTestInvisible) NewVisibility = ESlateVisibility::Hidden;
	else if (CurrentVisibility == ESlateVisibility::SelfHitTestInvisible) NewVisibility = ESlateVisibility::Hidden;
	WBP_InGameMenu->SetVisibility(NewVisibility);
}

void UUW_Layout_Cos::UpdatePileWidgetAmount(FGameplayTag PileTag, int32 NewAmount)
{
	// PileTag에 해당하는 위젯이 있는지 확인 후 수량을 업데이트
	if (PileWidgets.Contains(PileTag))
	{
		PileWidgets[PileTag]->UpdateAmountDirectly(NewAmount);
	}
}

void UUW_Layout_Cos::DisplayScreenLogMessage(const FText& Message, const FLinearColor& Color)
{
	// 메시지 노티 박스가 유효한지 검사 후 메시지를 표시
	WBP_MessageNotifyBox->ReceiveScreenLogMessage(Message, Color);
}

bool UUW_Layout_Cos::GetEndTurnButtonIsEnabled()
{
	return !WBP_CardHand->bCardInHand && bPlayerTurn;
}

ESlateVisibility UUW_Layout_Cos::GetCurrencyBoxVisibility()
{
	return WBP_CurrencyBar->StatusBoxVertical->GetChildrenCount() == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
}

void UUW_Layout_Cos::OnClicked_EndTurnButton()
{
	// 플레이어의 턴 종료
	bPlayerTurn = false;

	// 카드 플레이어를 가져와 유효성 검사 후 턴 종료 이벤트 호출
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	COS_IF_CHECK_VOID(CardPlayer, TEXT("CardPlayer를 찾을 수 없습니다."));

	// 턴 종료 이벤트 호출
	CardPlayer->DispatcherHubLocalComponent->CallEvent(CosGameTags::Event_TurnEnd, CardPlayer);
}

void UUW_Layout_Cos::OnClicked_DebugButton()
{
	// 현재 가시성에 따라 Visible 또는 Collapsed로 전환
	ESlateVisibility CurrentVisibility = WBP_DispatcherHubDebug->GetVisibility();

	ESlateVisibility NewVisibility = ESlateVisibility::Collapsed;
	if (CurrentVisibility == ESlateVisibility::Visible) NewVisibility = ESlateVisibility::Collapsed;
	else if (CurrentVisibility == ESlateVisibility::Collapsed) NewVisibility = ESlateVisibility::Visible;
	else if (CurrentVisibility == ESlateVisibility::Hidden) NewVisibility = ESlateVisibility::Collapsed;
	else if (CurrentVisibility == ESlateVisibility::HitTestInvisible) NewVisibility = ESlateVisibility::Collapsed;
	else if (CurrentVisibility == ESlateVisibility::SelfHitTestInvisible) NewVisibility = ESlateVisibility::Collapsed;

	// 디버그 허브와 카드 리스트 버튼의 가시성 변경
	WBP_DispatcherHubDebug->SetVisibility(NewVisibility);
	WBP_CardListButton_Void->SetVisibility(NewVisibility);
	WBP_CardListButton_Destroy->SetVisibility(NewVisibility);
}

void UUW_Layout_Cos::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_Action_NewPlayerTurn)
	{
		// 새로운 턴 시작
		bPlayerTurn = true;
	}
	else if (EventTag == CosGameTags::Event_Victory || EventTag == CosGameTags::Event_Action_GameOver || EventTag == CosGameTags::Event_Action_WinGame)
	{
		// 플레이어 턴 종료
		bPlayerTurn = false;

		if (EventTag == CosGameTags::Event_Action_GameOver)
		{
			// 게임 오버 처리
			WBP_GameOver->ShowGameOver(FText::FromString(TEXT("Game Over!")), FLinearColor::Red);
		}
		else if (EventTag == CosGameTags::Event_Action_WinGame)
		{
			// 게임 승리 처리
			WBP_GameOver->ShowGameOver(FText::FromString(TEXT("Victory!")), FLinearColor::Green);
		}
	}
	else if (EventTag == CosGameTags::Event_DeckSetupDone)
	{
		// 덱 설정 완료 시, 각 카드 리스트 버튼 업데이트
		for (const TTuple<FGameplayTag, UUW_CardListButton*>& PileWidget : PileWidgets)
		{
			PileWidget.Value->UpdateAmountFromPileRef();
		}
	}
}

void UUW_Layout_Cos::InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen)
{
	// 스토리 인카운터 UI 초기화 (기존 자식 제거)
	StoryEncounterBox->ClearChildren();

	// 게임 인스턴스 유효성 검사 및 가져오기
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	COS_IF_CHECK_VOID(CardGameInstance, TEXT("GameInstance는 UInterface_CardGameInstance를 상속받지 않았습니다"));
	
	if (!IsValid(EncounterData.DataTable))
	{
		IInterface_CardGameInstance::Execute_AttemptSaveGame(CardGameInstance,TEXT(""), true);
		return;
	}

	FStoryEncounter* StoryEncounter = EncounterData.DataTable->FindRow<FStoryEncounter>(EncounterData.RowName,TEXT(""));
	if (!StoryEncounter)
	{
		IInterface_CardGameInstance::Execute_AttemptSaveGame(CardGameInstance,TEXT(""), true);
		return;
	}

	// 스토리 위젯 생성 및 추가
	UUserWidget* NewStoryEncounter = CreateWidget<UUserWidget>(StoryEncounterBox, StoryEncounter->EncounterWidget);
	StoryEncounterBox->AddChild(NewStoryEncounter);

	COS_IF_CHECK_VOID(NewStoryEncounter->GetClass()->ImplementsInterface(UInterface_StoryEncounter::StaticClass()), TEXT("StoryWidget이 UInterface_StoryEncounter를 상속받지 않았습니다"));

	// 스토리 인카운터 초기화 함수 호출
	IInterface_StoryEncounter::Execute_InitializeStoryEncounter(NewStoryEncounter, EncounterData, bIsFirstScreen);
}

UObject* UUW_Layout_Cos::AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately)
{
	if (Status->SlotType == EStatusSlot::Currency)
	{
		return IInterface_StatusWidget::Execute_AddStatusIndicator(WBP_CurrencyBar, Status, false);
	}
	if (Status->SlotType == EStatusSlot::Artifact)
	{
		return IInterface_StatusWidget::Execute_AddStatusIndicator(WBP_ArtifactBar, Status, false);
	}

	return nullptr;
}

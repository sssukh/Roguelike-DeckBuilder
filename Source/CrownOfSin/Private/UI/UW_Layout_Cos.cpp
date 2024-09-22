#include "UI/UW_Layout_Cos.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_RewardScreen.h"
#include "UI/UW_ScreenFade.h"
#include "Utilities/CosLog.h"


UUW_Layout_Cos::UUW_Layout_Cos(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              DeckListSwitcher(nullptr), StoryEncounterBox(nullptr), DebugButton(nullptr), WBP_RewardScreen(nullptr),
                                                                              WBP_ScreenFade(nullptr),
                                                                              WBP_Shop(nullptr)
{
}

void UUW_Layout_Cos::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimationReverse(WBP_ScreenFade->Occlude, 0.5f);

	SetupDebugMode();

	UpdateHandAnchorWidgets();

	// SetupPileWidgetMap();

	UpdateHandAnchorWidgets();
}

void UUW_Layout_Cos::SetupDebugMode()
{
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	bool bIsDebugMode = IInterface_CardGameInstance::Execute_IsDebugMode(CardGameInstance);
	ESlateVisibility NewVisibility = bIsDebugMode ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	DebugButton->SetVisibility(NewVisibility);
}

void UUW_Layout_Cos::UpdateHandAnchorWidgets()
{
}

bool UUW_Layout_Cos::ShouldNodeMapBeBlocked()
{
	return WBP_RewardScreen->GetVisibility() == ESlateVisibility::Visible ||
		StoryEncounterBox->GetChildrenCount() > 0 ||
		DeckListSwitcher->IsVisible();
}

void UUW_Layout_Cos::UpdatePileWidgetAmount(FGameplayTag PileTag, int32 NewAmount)
{
	// COS_LOG_SCREEN(TEXT("UpdatePileWidgetAmount() 구현하세요"));
}

bool UUW_Layout_Cos::GetEndTurnButtonIsEnabled()
{
	// COS_LOG_SCREEN(TEXT("GetEndTurnButtonIsEnabled() 구현하세요"));
	return true;
}

ESlateVisibility UUW_Layout_Cos::GetCurrencyBoxVisibility()
{
	/*ToDo 구현해야합니다.*/
	// COS_LOG_SCREEN(TEXT("GetCurrencyBoxVisibility()함수를 구현해야합니다"));
	return ESlateVisibility::Visible;
}

void UUW_Layout_Cos::InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen)
{
	StoryEncounterBox->ClearChildren();

	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(this);
	if (!gameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
		return;
	}

	if (!IsValid(EncounterData.DataTable))
	{
		IInterface_CardGameInstance::Execute_AttemptSaveGame(gameInstance,TEXT(""), true);
		return;
	}

	FStoryEncounter* StoryEncounter = EncounterData.DataTable->FindRow<FStoryEncounter>(EncounterData.RowName,TEXT("FStoryEncounter in UW_Layout_Cos"));
	if (!StoryEncounter)
	{
		IInterface_CardGameInstance::Execute_AttemptSaveGame(gameInstance,TEXT(""), true);
		return;
	}

	UUserWidget* NewStoryEncounterWidget = CreateWidget<UUserWidget>(StoryEncounterBox, StoryEncounter->EncounterWidget);
	StoryEncounterBox->AddChild(NewStoryEncounterWidget);

	// 생성된 위젯의 Interface상속 확인
	if (!NewStoryEncounterWidget->GetClass()->ImplementsInterface(UInterface_StoryEncounter::StaticClass()))
	{
		COS_SCREEN(TEXT("StoryWidget이 UInterface_StoryEncounter를 상속받지 않았습니다"));
		return;
	}
	Execute_InitializeStoryEncounter(NewStoryEncounterWidget, EncounterData, bIsFirstScreen);
}

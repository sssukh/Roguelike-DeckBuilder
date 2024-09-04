#include "UI/UW_Layout_Cos.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UW_RewardScreen.h"
#include "UI/UW_ScreenFade.h"
#include "Utilities/CosLog.h"


UUW_Layout_Cos::UUW_Layout_Cos(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                              DeckListSwitcher(nullptr), StoryEncounterBox(nullptr), WBP_RewardScreen(nullptr)
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
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		bool bIsDebugMode = IInterface_CardGameInstance::Execute_IsDebugMode(GameInstance);
		ESlateVisibility NewVisibility = bIsDebugMode ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
		DebugButton->SetVisibility(NewVisibility);
	}
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

void UUW_Layout_Cos::InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen)
{
	// StoryEncounterBox에 원래 있던 것들 정리
	StoryEncounterBox->ClearChildren();

	
	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(this);
	if(!gameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
		return;
	}
	
	// 받아온 EcounterData의 DataTable이 유효하지 않으면 저장하고 끝
	if(!IsValid(EncounterData.DataTable))
	{
		IInterface_CardGameInstance::Execute_AttemptSaveGame(gameInstance,TEXT(""),true);
		return;
	}

	FStoryEncounter* StoryEncounter = EncounterData.DataTable->FindRow<FStoryEncounter>(EncounterData.RowName,TEXT("FStoryEncounter in UW_Layout_Cos"));

	// 받아온 EncounterData로 데이터를 받아올 수 없으면 저장하고 끝
	if(!StoryEncounter)
	{
		IInterface_CardGameInstance::Execute_AttemptSaveGame(gameInstance,TEXT(""),true);
		return;
	}

	// 받아온 EncounterData로 데이터를 정상적으로 받아오면 스토리 위젯생성
	// 아마 WBP_StoryEncounter인듯하다.
	UUserWidget* StoryWidget = CreateWidget<UUserWidget>(StoryEncounterBox, StoryEncounter->EncounterWidget);

	// 받아온 정보로 만든 스토리 위젯을 스토리 인카운터 박스에 자식으로 추가
	StoryEncounterBox->AddChild(StoryWidget);

	// 생성된 위젯의 Interface상속 확인
	if(!StoryWidget->GetClass()->ImplementsInterface(UInterface_StoryEncounter::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("StoryWidget이 UInterface_StoryEncounter를 상속받지 않았습니다"));
		return;
	}

	IInterface_StoryEncounter::Execute_InitializeStoryEncounter(StoryWidget,EncounterData,bIsFirstScreen);
	
}

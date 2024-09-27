#include "UI/UW_MinionUI.h"

#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "StatusSystem/StatusComponent.h"
#include "UI/UW_StatBar.h"
#include "UI/UW_StatusIcon.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UUW_MinionUI::UUW_MinionUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_MinionUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(ActiveStatusWobble, 0, 0, EUMGSequencePlayMode::PingPong, 1.0f);
	PlayAnimation(FrameZoom, 0, 0, EUMGSequencePlayMode::PingPong, 1.0f);
}

UObject* UUW_MinionUI::AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately)
{
	COS_IF_CHECK_RETURN(WBP_StatusIconClass, TEXT("UUW_MinionUI에서 WBP_StatusIconClass를 설정해주세요"), nullptr);
	COS_IF_CHECK_RETURN(WBP_StatBar, TEXT("UUW_MinionUI에서 WBP_StatBar를 설정해주세요"), nullptr);

	// 상태에 따른 위젯 클래스를 선택
	TSubclassOf<UUserWidget> StatusWidgetClass = nullptr;
	switch (Status->SlotType)
	{
	case EStatusSlot::Active:
	case EStatusSlot::Passive:
	case EStatusSlot::OverlapBar:
		StatusWidgetClass = WBP_StatusIconClass;
		break;
	case EStatusSlot::Bar:
		StatusWidgetClass = WBP_StatBarClass;
		break;
	default:
		return nullptr;
	}

	// 선택된 위젯 클래스가 유효한지 검증
	if (!IsValid(StatusWidgetClass))
	{
		return nullptr;
	}

	// 위젯 생성
	UUserWidget* NewStatusWidget = CreateWidget(GetOwningPlayer(), StatusWidgetClass);

	// 상태에 따른 패널 선택
	UPanelWidget* TargetPanel = nullptr;
	switch (Status->SlotType)
	{
	case EStatusSlot::Active:
		TargetPanel = ActiveStatusBox;
		break;
	case EStatusSlot::Passive:
		TargetPanel = PassiveStatusBox;
		break;
	case EStatusSlot::OverlapBar:
		TargetPanel = OverlapBarBox;
		break;
	case EStatusSlot::Bar:
		TargetPanel = StatBarBox;
		break;
	default:
		return nullptr;
	}

	// 선택된 패널에 위젯 추가
	if (TargetPanel)
	{
		TargetPanel->AddChild(NewStatusWidget);
	}
	
	// 생성된 위젯 초기화
	IInterface_StatusIcon::Execute_InitStatusWidget(NewStatusWidget, Status, CosGameTags::StatusBar_MinionBar, bShowImmediately);

	return NewStatusWidget;
}

bool UUW_MinionUI::Interact_Implementation(const FGameplayTagContainer& Tags)
{
	// 태그 배열을 가져옴
	const TArray<FGameplayTag>& TagsArray = Tags.GetGameplayTagArray();

	// 모든 태그를 순회하여 처리
	for (const FGameplayTag& Tag : TagsArray)
	{
		if (Tag == CosGameTags::Interact_Mark)
		{
			TargetFrame->SetVisibility(ESlateVisibility::Visible);
			return true; // 해당 태그 처리 후 반환
		}

		if (Tag == CosGameTags::Interact_UnMark)
		{
			TargetFrame->SetVisibility(ESlateVisibility::Hidden);
			return true; // 해당 태그 처리 후 반환
		}
	}

	// 해당 태그가 없으면 true 반환
	return true;
}

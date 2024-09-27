#include "UI/UW_StatusBar.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

#include "Interfaces/Interface_Utility.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/StatusComponent.h"
#include "UI/UW_StatusIcon.h"
#include "Utilities/CosLog.h"
#include "Components/PanelWidget.h"

UUW_StatusBar::UUW_StatusBar(const FObjectInitializer& Initializer) : Super(Initializer), StatusBoxHorizontal(nullptr), StatusBoxVertical(nullptr)
{
	static ConstructorHelpers::FClassFinder<UUW_StatusIcon> WBP_StatusIcon(*AssetPath::Blueprint::WBP_StatusIcon_C);
	if (WBP_StatusIcon.Succeeded())
	{
		WBP_StatusIconClass = WBP_StatusIcon.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_StatusIcon를 로드하지 못했습니다."));
	}
}

void UUW_StatusBar::ClearAllStatuses()
{
	// bVertical 플래그에 따라 적절한 StatusBox 선택 후 모든 자식 위젯 제거
	if (bVertical)
	{
		StatusBoxVertical->ClearChildren();
	}
	else
	{
		StatusBoxHorizontal->ClearChildren();
	}
}

int32 UUW_StatusBar::GetStatusCount()
{
	// bVertical 플래그에 따라 해당 StatusBox의 자식 개수 반환
	if (bVertical)
	{
		return StatusBoxVertical->GetChildrenCount();
	}

	return StatusBoxHorizontal->GetChildrenCount();
}

bool UUW_StatusBar::GetNumberOfVisibleRewards(int32& OutVisibleRewardCount)
{
	// 위젯이 가시 상태가 아닐 경우 -1을 반환하고 false를 리턴
	if (!IsVisible())
	{
		OutVisibleRewardCount = -1;
		return false;
	}

	// 가시 상태의 아이콘을 세기 위한 변수
	int32 NumVisibleRewards = 0;

	// bVertical 플래그에 따라 각각 StatusBox의 자식 위젯에서 가시 상태의 위젯을 카운트
	TArray<UWidget*> ChildrenWidgets;
	if (bVertical)
	{
		ChildrenWidgets = StatusBoxVertical->GetAllChildren();
	}
	else
	{
		ChildrenWidgets = StatusBoxHorizontal->GetAllChildren();
	}

	for (UWidget* ChildrenWidget : ChildrenWidgets)
	{
		if (ChildrenWidget->IsVisible())
		{
			NumVisibleRewards++;
		}
	}

	// 결과를 OutVisibleRewardCount에 저장
	OutVisibleRewardCount = NumVisibleRewards;

	// 가시 상태의 아이콘이 하나라도 있으면 true를 반환, 아니면 false
	return NumVisibleRewards > 0;
}

void UUW_StatusBar::InteractWithAllStatuses()
{
	// bVertical에 따라 적절한 StatusBox에서 자식 위젯을 가져옴
	UPanelWidget* StatusBox = bVertical ? Cast<UPanelWidget>(StatusBoxVertical) : Cast<UPanelWidget>(StatusBoxHorizontal);
	const TArray<UWidget*>& ChildrenWidgets = StatusBox->GetAllChildren();

	// 각 위젯에 대해 상호작용 처리
	for (UWidget* Widget : ChildrenWidgets)
	{
		// 위젯이 UUW_StatusIcon 타입인지 확인
		UUW_StatusIcon* StatusIconWidget = Cast<UUW_StatusIcon>(Widget);
		if (StatusIconWidget && StatusIconWidget->IsVisible())
		{
			// 가시 상태인 경우 상호작용 수행
			IInterface_Utility::Execute_Interact(StatusIconWidget->StatusComponent, FGameplayTagContainer(StatusBarTag));
		}
	}
}

UObject* UUW_StatusBar::AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately)
{
	// WBP_StatusIconClass가 설정되어 있는지 확인
	COS_IF_CHECK_RETURN(WBP_StatusIconClass, TEXT("UUW_StatusBar에서 WBP_StatusIconClass를 설정해주세요."), nullptr);

	// 새로운 상태 아이콘 위젯 생성
	UUW_StatusIcon* StatusIconWidget = CreateWidget<UUW_StatusIcon>(GetOwningPlayer(), WBP_StatusIconClass);

	// bVertical에 따라 적절한 레이아웃에 아이콘 추가
	UPanelWidget* StatusBox = bVertical ? Cast<UPanelWidget>(StatusBoxVertical) : Cast<UPanelWidget>(StatusBoxHorizontal);
	StatusBox->AddChild(StatusIconWidget);

	// 상태 아이콘이 UInterface_StatusIcon 인터페이스를 구현하는지 확인 후 초기화
	if (StatusIconWidget->GetClass()->ImplementsInterface(UInterface_StatusIcon::StaticClass()))
	{
		IInterface_StatusIcon::Execute_InitStatusWidget(StatusIconWidget, Status, StatusBarTag, bShowImmediately);
	}

	return StatusIconWidget;
}

#include "UI/UW_StatusBar.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Interfaces/Interface_Utility.h"
#include "Libraries/AssetTableRef.h"
#include "StatusSystem/StatusComponent.h"
#include "UI/UW_StatusIcon.h"

UUW_StatusBar::UUW_StatusBar(const FObjectInitializer& Initializer) : Super(Initializer), StatusBoxHorizontal(nullptr), StatusBoxVertical(nullptr)
{
	if (TSubclassOf<UUW_StatusIcon> WBP_StatusIcon = FAssetReferenceUtility::FindClassFromDataTable<UUW_StatusIcon>(AssetRefPath::BluePrintPath, FName("WBP_StatusIcon"), true))
	{
		WBP_StatusIconClass = WBP_StatusIcon;
	}
}

void UUW_StatusBar::ClearAllStatuses()
{
	if (bVertical)
		StatusBoxVertical->ClearChildren();
	else
		StatusBoxHorizontal->ClearChildren();
}

int32 UUW_StatusBar::GetStatusCount()
{
	return StatusBoxHorizontal->GetChildrenCount() + StatusBoxVertical->GetChildrenCount();
}

bool UUW_StatusBar::GetNumberOfVisibleRewards(int32& OutVisibleRewardCount)
{
	if (IsVisible())
	{
		TArray<UWidget*> ChildrenWidgets;
		if (bVertical)
			ChildrenWidgets = StatusBoxVertical->GetAllChildren();
		else
			ChildrenWidgets = StatusBoxHorizontal->GetAllChildren();

		int32 VisibleRewardCount = 0;
		for (UWidget* ChildrenWidget : ChildrenWidgets)
		{
			if (ChildrenWidget->IsVisible())
			{
				VisibleRewardCount++;
			}
		}

		OutVisibleRewardCount = VisibleRewardCount;
		return true;
	}

	OutVisibleRewardCount = -1;
	return false;
}

void UUW_StatusBar::InteractWithAllStatuses()
{
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
		UUW_StatusIcon* UW_StatusIcon = Cast<UUW_StatusIcon>(ChildrenWidget);
		if (!UW_StatusIcon)
			continue;

		if (UW_StatusIcon->IsVisible())
		{
			IInterface_Utility::Execute_Interact(UW_StatusIcon->StatusComponent,FGameplayTagContainer(StatusBarTag));
		}
	}
}

UObject* UUW_StatusBar::AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately)
{
	UUW_StatusIcon* NewStatusIconWidget = CreateWidget<UUW_StatusIcon>(GetOwningPlayer(), WBP_StatusIconClass);

	if (bVertical)
		StatusBoxVertical->AddChild(NewStatusIconWidget);
	else
		StatusBoxHorizontal->AddChild(NewStatusIconWidget);

	if (NewStatusIconWidget->GetClass()->ImplementsInterface(UInterface_StatusIcon::StaticClass()))
		IInterface_StatusIcon::Execute_InitStatusWidget(NewStatusIconWidget, Status, StatusBarTag, false);

	return NewStatusIconWidget;
}

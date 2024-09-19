#include "UI/UW_ToolTipList.h"

#include "Components/VerticalBox.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_ToolTip.h"
#include "Utilities/CosLog.h"

UUW_ToolTipList::UUW_ToolTipList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), ToolTipBox(nullptr)
{
	static ConstructorHelpers::FClassFinder<UUW_ToolTip> WBP_ToolTip(*AssetPath::Blueprint::WBP_ToolTip_C);
	if (WBP_ToolTip.Succeeded())
	{
		WBP_ToolTipClass = WBP_ToolTip.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_ToolTip를 로드하지 못했습니다."));
	}
}

void UUW_ToolTipList::AddToolTipFromData(const FDataTableRowHandle& ToolTipData, int32 Value)
{
	if (UUW_ToolTip* NewToolTip = CreateWidget<UUW_ToolTip>(GetWorld(), WBP_ToolTipClass))
	{
		ToolTipBox->AddChild(NewToolTip);
	}
}

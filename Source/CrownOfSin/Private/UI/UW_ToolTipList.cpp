#include "UI/UW_ToolTipList.h"

#include "Components/VerticalBox.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_ToolTip.h"
#include "Utilities/CosLog.h"

UUW_ToolTipList::UUW_ToolTipList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), ToolTipBox(nullptr)
{
	static ConstructorHelpers::FClassFinder<UUW_ToolTip> WBP_ToolTip_C(*AssetPath::Blueprint::WBP_ToolTip_C);
	if (WBP_ToolTip_C.Succeeded())
	{
		WBP_ToolTipClass = WBP_ToolTip_C.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_ToolTip를 로드하지 못했습니다."));
	}
}

void UUW_ToolTipList::AddToolTipFromData(const FDataTableRowHandle& ToolTipData, int32 Value)
{
	COS_IF_CHECK(WBP_ToolTipClass, TEXT("UUW_ToolTipList에서 WBP_ToolTipClass를 설정해주세요."))

	if (UUW_ToolTip* NewToolTip = CreateWidget<UUW_ToolTip>(GetWorld(), WBP_ToolTipClass))
	{
		NewToolTip->ToolTipData = ToolTipData;
		NewToolTip->Value = Value;
		NewToolTip->TextOverride = FText();
		ToolTipBox->AddChild(NewToolTip);
	}
}

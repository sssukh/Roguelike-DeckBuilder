#include "UI/UW_ToolTipList.h"

#include "Components/VerticalBox.h"
#include "Libraries/AssetTableRef.h"
#include "UI/UW_ToolTip.h"

UUW_ToolTipList::UUW_ToolTipList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), ToolTipBox(nullptr)
{
	if (TSubclassOf<UUW_ToolTip> FoundToolTipClass = FAssetReferenceUtility::FindClassFromDataTable<UUW_ToolTip>(AssetRefPath::BluePrintPath, FName("WBP_Tooltip"), true))
	{
		WBP_ToolTipClass = FoundToolTipClass;
	}
}

void UUW_ToolTipList::AddToolTipFromData(const FDataTableRowHandle& ToolTipData, int32 Value)
{
	if (UUW_ToolTip* NewToolTip = CreateWidget<UUW_ToolTip>(GetWorld(), WBP_ToolTipClass))
	{
		ToolTipBox->AddChild(NewToolTip);
	}
}

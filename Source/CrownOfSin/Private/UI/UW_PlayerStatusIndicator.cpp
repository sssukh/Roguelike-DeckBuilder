#include "UI/UW_PlayerStatusIndicator.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/StatusComponent.h"
#include "StatusSystem/Status_Mana.h"
#include "UI/UW_ToolTip.h"
#include "UI/UW_ToolTipList.h"
#include "Utilities/CosLog.h"

UUW_PlayerStatusIndicator::UUW_PlayerStatusIndicator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), StatText(nullptr), IncreaseAnim(nullptr), StatComponent(nullptr)
{
	StatusClass = UStatus_Mana::StaticClass();

	static ConstructorHelpers::FClassFinder<UUW_ToolTip> WBP_ToolTip(*AssetPath::Blueprint::WBP_ToolTip_C);
	if (WBP_ToolTip.Succeeded())
	{
		WBP_ToolTipClass = WBP_ToolTip.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_ToolTipClass를 로드하지 못했습니다."));
	}

	static ConstructorHelpers::FClassFinder<UUW_ToolTipList> WBP_ToolTipList(*AssetPath::Blueprint::WBP_ToolTipList_C);
	if (WBP_ToolTipList.Succeeded())
	{
		WBP_ToolTipListClass = WBP_ToolTipList.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_ToolTipListClass 로드하지 못했습니다."));
	}
}

void UUW_PlayerStatusIndicator::UpdateValue(int32 NewValue)
{
	StatText->SetText(UKismetTextLibrary::Conv_IntToText(NewValue));
	PlayAnimation(IncreaseAnim, 0, 1, EUMGSequencePlayMode::PingPong);
}

void UUW_PlayerStatusIndicator::UpdateTooltipFromStatus(UStatusComponent* InStatus)
{
	int32 ToolTipsNum = InStatus->Tooltips.Num();

	if (ToolTipsNum == 0) return;

	COS_IF_CHECK_VOID(WBP_ToolTipClass, TEXT("WBP_ToolTipClass를 설정해주세요"));
	if (ToolTipsNum == 1)
	{
		UUW_ToolTip* NewToolTip = CreateWidget<UUW_ToolTip>(GetWorld(), WBP_ToolTipClass);
		NewToolTip->ToolTipData = InStatus->Tooltips[0].ToolTipTable;
		NewToolTip->Value = InStatus->Tooltips[0].bValued ? InStatus->StatusValue : 0;
		NewToolTip->TextOverride = FText::FromString(TEXT(""));
		SetToolTip(NewToolTip);
		return;
	}

	//ToolTipNum >1
	COS_IF_CHECK_VOID(WBP_ToolTipListClass, TEXT("WBP_ToolTipListClass를 설정해주세요"));
	UUW_ToolTipList* NewToolTipList = CreateWidget<UUW_ToolTipList>(GetWorld(), WBP_ToolTipListClass);
	SetToolTip(NewToolTipList);
	for (const FToolTipValue& Tooltip : InStatus->Tooltips)
	{
		NewToolTipList->AddToolTipFromData(Tooltip.ToolTipTable, Tooltip.bValued ? InStatus->StatusValue : 0);
	}
}

void UUW_PlayerStatusIndicator::InitStatusWidget_Implementation(UStatusComponent* InStatus, FGameplayTag InStatusBarTag, bool bIsShowImmediately)
{
	StatComponent = InStatus;
	UpdateValue(StatComponent->StatusValue);
	StatComponent->StatusIndicator = this;
	UpdateTooltipFromStatus(StatComponent);
}

bool UUW_PlayerStatusIndicator::ModifyStatusWidget_Implementation(int32 NewValue, const FText& TextOverride, bool bRefreshAppearance, const FStatusAppearance& StatusAppearanceIn, bool bCanBeZero)
{
	UpdateValue(NewValue);
	return true;
}

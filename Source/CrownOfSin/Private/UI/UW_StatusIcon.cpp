﻿#include "UI/UW_StatusIcon.h"

#include "UI/UW_ToolTip.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/Interface_Utility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/StatusComponent.h"
#include "UI/UW_ToolTipList.h"
#include "Utilities/CosLog.h"

UUW_StatusIcon::UUW_StatusIcon(const FObjectInitializer& Initializer) : Super(Initializer), StatusButton(nullptr), BottomRightNumber(nullptr), CenterNumber(nullptr), BottomLeftNumber(nullptr),
                                                                        AlertModify(nullptr),
                                                                        FlyUp(nullptr),
                                                                        NoticeWobble(nullptr), FadeOut(nullptr),
                                                                        StatusComponent(nullptr),
                                                                        StatusAppearance()
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


	static ConstructorHelpers::FClassFinder<UUW_ToolTipList> WBP_ToolTipList(*AssetPath::Blueprint::WBP_ToolTipList_C);
	if (WBP_ToolTipList.Succeeded())
	{
		WBP_ToolTipListClass = WBP_ToolTipList.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_ToolTipList를 로드하지 못했습니다."));
	}
}

void UUW_StatusIcon::NativeConstruct()
{
	Super::NativeConstruct();


	StatusButton->OnClicked.AddDynamic(this, &UUW_StatusIcon::OnClicked_StatusButton);
}

void UUW_StatusIcon::OnClicked_StatusButton()
{
	IInterface_Utility::Execute_Interact(StatusComponent, FGameplayTagContainer(StatusBarTag));
}

void UUW_StatusIcon::RefreshAppearance()
{
	UpdateAppearance();
	UpdateTooltipFromStatus();
	AlignValueTextFromStatus();
}

void UUW_StatusIcon::UpdateTooltipFromStatus()
{
	if (StatusAppearance.Tooltips.IsEmpty())
	{
		return;
	}

	if (StatusAppearance.Tooltips.Num() == 1)
	{
		if (UUW_ToolTip* NewToolTip = CreateWidget<UUW_ToolTip>(GetWorld(), WBP_ToolTipClass))
		{
			NewToolTip->ToolTipData = StatusAppearance.Tooltips[0].ToolTipTable;
			NewToolTip->Value = StatusAppearance.Tooltips[0].bValued ? StatusAppearance.StatusValue : 0;
			SetToolTip(NewToolTip);
		}
		return;
	}

	if (StatusAppearance.Tooltips.Num() > 1)
	{
		if (UUW_ToolTipList* NewToolTipList = CreateWidget<UUW_ToolTipList>(GetWorld(), WBP_ToolTipListClass))
		{
			SetToolTip(NewToolTipList);

			for (const FToolTipValue& Tooltip : StatusAppearance.Tooltips)
			{
				int32 ToolTipValue = Tooltip.bValued ? StatusAppearance.StatusValue : 0;
				NewToolTipList->AddToolTipFromData(Tooltip.ToolTipTable, ToolTipValue);
			}
		}
	}
}

void UUW_StatusIcon::InitStatusWidget_Implementation(UStatusComponent* InStatus, FGameplayTag InStatusBarTag, bool bIsShowImmediately)
{
	StatusComponent = InStatus;

	StatusAppearance = StatusComponent->MakeAppearanceStruct();

	StatusBarTag = InStatusBarTag;

	UpdateAppearance();

	UpdateTooltipFromStatus();

	AlignValueTextFromStatus();

	if (bIsShowImmediately)
		SetVisibility(ESlateVisibility::Visible);
}

bool UUW_StatusIcon::ModifyStatusWidget_Implementation(int32 NewValue, const FText& TextOverride, bool bRefreshAppearance, const FStatusAppearance& StatusAppearanceIn, bool bCanBeZero)
{
	// RewardBarText는 숨겨진 보상을 나타냄
	static const FText RewardBarText = FText::FromString(TEXT("HideReward"));

	// 유효한 값인지 확인
	const bool bShouldShow = NewValue > 0 || bCanBeZero;
	if (bShouldShow)
	{
		SetVisibility(ESlateVisibility::Visible);

		StatusAppearance = StatusAppearanceIn;

		if (bRefreshAppearance)
			RefreshAppearance();
		else
			UpdateTooltipFromStatus();

		FText TextValue = TextOverride.IsEmpty() ? UKismetTextLibrary::Conv_IntToText(NewValue) : TextOverride;
		BottomLeftNumber->SetText(TextValue);
		BottomRightNumber->SetText(TextValue);
		CenterNumber->SetText(TextValue);

		if (StatusAppearance.SlotType == EStatusSlot::Active)
		{
			PlayAnimation(FlyUp);
		}
		else if (StatusAppearance.SlotType == EStatusSlot::None || StatusAppearance.SlotType == EStatusSlot::Passive ||
			StatusAppearance.SlotType == EStatusSlot::OverlapBar || StatusAppearance.SlotType == EStatusSlot::Bar)
		{
			PlayAnimation(AlertModify);
		}

		// 특정 시간이 지나면 추가 애니메이션 실행
		if (UGameplayStatics::GetTimeSeconds(this) > 0.1f)
		{
			PlayAnimation(NoticeWobble);
		}
	}
	else
	{
		// 텍스트가 'HideReward'와 동일한 경우 애니메이션 처리
		const bool bIsRewardText = UKismetTextLibrary::EqualEqual_IgnoreCase_TextText(TextOverride, RewardBarText);
		if (bIsRewardText)
		{
			PlayAnimation(FadeOut);
		}
		else
		{
			FadeAway(true);
		}
	}

	return true;
}

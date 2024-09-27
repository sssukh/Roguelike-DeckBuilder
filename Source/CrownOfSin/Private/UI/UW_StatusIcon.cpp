#include "UI/UW_StatusIcon.h"

#include "Animation/WidgetAnimation.h"
#include "UI/UW_ToolTip.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
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

void UUW_StatusIcon::UpdateFontSize(UTextBlock* Text, float RelativeScale)
{
	// 텍스트 블록의 현재 폰트 정보를 가져옴
	FSlateFontInfo ScaledFontInfo = Text->GetFont();

	// 상대적인 스케일로 폰트 크기를 조정
	ScaledFontInfo.Size = FMath::TruncToInt(ScaledFontInfo.Size * RelativeScale);

	// 폰트 정보 업데이트
	Text->SetFont(ScaledFontInfo);
}

void UUW_StatusIcon::UpdateSizeBoxSize(USizeBox* SizeBox, float RelativeScale)
{
	SizeBox->SetWidthOverride(SizeBox->GetWidthOverride() * RelativeScale);
	SizeBox->SetHeightOverride(SizeBox->GetHeightOverride() * RelativeScale);
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

	// ToolTip이 하나일 때 처리
	if (StatusAppearance.Tooltips.Num() == 1)
	{
		COS_IF_CHECK_VOID(WBP_ToolTipClass, TEXT("UW_StatusIcon에서 WBP_ToolTipClass를 설정해주세요."));
		if (UUW_ToolTip* NewToolTipWidget = CreateWidget<UUW_ToolTip>(GetWorld(), WBP_ToolTipClass))
		{
			NewToolTipWidget->ToolTipData = StatusAppearance.Tooltips[0].ToolTipTable;
			NewToolTipWidget->Value = StatusAppearance.Tooltips[0].bValued ? StatusAppearance.StatusValue : 0;
			SetToolTip(NewToolTipWidget);
		}
	}
	// ToolTip이 여러 개일 때 처리
	else if (StatusAppearance.Tooltips.Num() > 1)
	{
		COS_IF_CHECK_VOID(WBP_ToolTipListClass, TEXT("UW_StatusIcon에서 WBP_ToolTipClass를 설정해주세요."));
		if (UUW_ToolTipList* ToolTipListWidget = CreateWidget<UUW_ToolTipList>(GetWorld(), WBP_ToolTipListClass))
		{
			SetToolTip(ToolTipListWidget);

			for (const FToolTipValue& Tooltip : StatusAppearance.Tooltips)
			{
				int32 ToolTipValue = Tooltip.bValued ? StatusAppearance.StatusValue : 0;
				ToolTipListWidget->AddToolTipFromData(Tooltip.ToolTipTable, ToolTipValue);
			}
		}
	}
}

void UUW_StatusIcon::FadeAway(bool bDestroyAfterFade)
{
	PlayAnimation(FadeOut);
	FTimerHandle FadeOutTimerHandle;

	float FadeOutDelay = FadeOut->GetEndTime();
	GetWorld()->GetTimerManager().SetTimer(FadeOutTimerHandle, [&]()
	{
		if (bDestroyAfterFade)
		{
			RemoveFromParent();
		}
		else
		{
			SetRenderOpacity(0.0f);
		}
	}, FadeOutDelay, false);
}

void UUW_StatusIcon::AlignValueTextFromStatus()
{
	// 모든 텍스트 블록을 숨김 처리
	BottomRightNumber->SetVisibility(ESlateVisibility::Hidden);
	CenterNumber->SetVisibility(ESlateVisibility::Hidden);
	BottomLeftNumber->SetVisibility(ESlateVisibility::Hidden);

	// 정렬 방식에 따라 텍스트를 설정하고 해당 블록만 표시
	UTextBlock* TargetTextBlock;

	switch (StatusAppearance.TextAlignment)
	{
	case EStatusTextAlignment::Center:
		TargetTextBlock = CenterNumber;
		break;

	case EStatusTextAlignment::BottomLeft:
		TargetTextBlock = BottomLeftNumber;
		break;

	case EStatusTextAlignment::BottomRight:
		TargetTextBlock = BottomRightNumber;
		break;

	default:
		return; // None인 경우 텍스트를 표시하지 않음
	}

	if (TargetTextBlock)
	{
		TargetTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(StatusAppearance.StatusValue));
		TargetTextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // 상수 사용
	}
}

void UUW_StatusIcon::UpdateAppearance()
{
	StatusImage->SetBrushFromTexture(StatusAppearance.Icon);
	StatusImage->SetBrushTintColor(StatusAppearance.Tint);
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
	// 상태를 표시할지 결정하는 플래그
	const bool bShouldShow = NewValue > 0 || bCanBeZero;
	if (bShouldShow)
	{
		SetVisibility(ESlateVisibility::Visible);
		StatusAppearance = StatusAppearanceIn;

		// 외형 새로고침
		if (bRefreshAppearance)
		{
			RefreshAppearance();
		}
		else
		{
			UpdateTooltipFromStatus();
		}

		// 텍스트 값을 설정 (텍스트 오버라이드가 있으면 그 값을 사용)
		FText FinalTextValue = TextOverride.IsEmpty() ? UKismetTextLibrary::Conv_IntToText(NewValue) : TextOverride;

		// 텍스트 설정을 배열로 처리하여 중복 제거
		TArray<UTextBlock*> TextBlocks = {BottomLeftNumber, BottomRightNumber, CenterNumber};
		for (UTextBlock* TextBlock : TextBlocks)
		{
			TextBlock->SetText(FinalTextValue);
		}

		// SlotType에 따른 애니메이션 설정
		switch (StatusAppearance.SlotType)
		{
		case EStatusSlot::None:
		case EStatusSlot::Passive:
		case EStatusSlot::Bar:
		case EStatusSlot::OverlapBar:
			PlayAnimation(AlertModify);
			break;
		case EStatusSlot::Active:
			PlayAnimation(FlyUp);
			break;
		default: break;;
		}

		// 추가 애니메이션 실행 조건 (시간 상수로 처리)
		constexpr float TimeThreshold = 0.1f;
		if (UGameplayStatics::GetTimeSeconds(this) > TimeThreshold)
		{
			PlayAnimation(NoticeWobble);
		}
	}
	else
	{
		// 숨겨진 보상 텍스트
		static const FText RewardBarText = FText::FromString(TEXT("HideReward"));

		// 숨겨진 보상 텍스트인지 확인 후 애니메이션 처리
		if (UKismetTextLibrary::EqualEqual_IgnoreCase_TextText(TextOverride, RewardBarText))
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

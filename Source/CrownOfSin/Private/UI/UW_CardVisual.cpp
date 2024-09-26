#include "UI/UW_CardVisual.h"

#include "CardSystem/CardBase.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/Status_Mana.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UUW_CardVisual::UUW_CardVisual(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TypeNames.Add(CosGameTags::Effect_Attack, FText::FromString(FString(TEXT("Attack"))));
	TypeNames.Add(CosGameTags::Effect_Skill, FText::FromString(FString(TEXT("Skill"))));
	TypeNames.Add(CosGameTags::Effect_Power, FText::FromString(FString(TEXT("Power"))));
	TypeNames.Add(CosGameTags::Effect_Invalid, FText::FromString(FString(TEXT(""))));
	TypeNames.Add(CosGameTags::Effect_Curse, FText::FromString(FString(TEXT("Curse"))));

	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Transparent(*AssetPath::Texture::T_Transparent);
	if (T_Transparent.Succeeded())
	{
		DefaultRarityGemTexture = T_Transparent.Object;
	}
	else
	{
		COS_SCREEN(TEXT("T_Transparent를 로드하지 못했습니다."));
	}
}

void UUW_CardVisual::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(CardActor))
	{
		IInterface_CardWidget::Execute_UpdateCardWidget(this, CardActor);
	}
}

void UUW_CardVisual::ReplacePowerValues()
{
	// 카드 설명 텍스트를 가져와 로컬 변수에 저장
	FString LocalDescriptionString = CardActor->GetCardDescription(ECardDataType::Hand).ToString();

	// 강조 텍스트 태그 정의
	const FString BoldOpenTag = TEXT("<b>");
	const FString BoldCloseTag = TEXT("</>");
	const FString LineBreakTag = TEXT("<br>");
	const FString NewLine = TEXT("\r\n");

	// 카드 효과 값을 설명 텍스트에서 대체
	const TArray<FCardEffect>& CardEffects = CardActor->GetCardEffects(ECardDataType::Hand);
	int32 EffectIndex = 0;
	for (const FCardEffect& CardEffect : CardEffects)
	{
		FString FromPattern = FString(TEXT("*")) + FString::FromInt(EffectIndex);
		FString EffectValueString = FString::FromInt(CardEffect.EffectValue);
		FString ToPattern = BoldOpenTag + EffectValueString + BoldCloseTag;
		LocalDescriptionString = LocalDescriptionString.Replace(*FromPattern, *ToPattern, ESearchCase::IgnoreCase);
		EffectIndex++;
	}

	// 카드 시작 상태 값을 설명 텍스트에서 대체
	const TArray<FStatusData>& StartingStatuses = CardActor->GetCardStartingStatuses(ECardDataType::Hand);
	int32 StatusIndex = 0;
	for (const FStatusData& StartingStatus : StartingStatuses)
	{
		FString FromPattern = FString(TEXT("#")) + FString::FromInt(StatusIndex);
		FString StatusValueString = FString::FromInt(StartingStatus.Value);
		FString ToPattern = BoldOpenTag + StatusValueString + BoldCloseTag;
		LocalDescriptionString = LocalDescriptionString.Replace(*FromPattern, *ToPattern, ESearchCase::IgnoreCase);
		StatusIndex++;
	}

	// `<br>`를 줄바꿈 문자열로 대체
	LocalDescriptionString = LocalDescriptionString.Replace(*LineBreakTag, *NewLine, ESearchCase::IgnoreCase);

	// 최종 텍스트를 UI에 반영
	RichDescription->SetText(FText::FromString(LocalDescriptionString));
}

void UUW_CardVisual::UpdateManaCrystal()
{
	// 카드의 핸드 상태에서 마나 비용을 가져옴
	int32 HandManaCost, HandUseRuleIndex;
	if (CardActor->GetStatusCostValueFromUseRules(ECardDataType::Hand, UStatus_Mana::StaticClass(), HandManaCost, HandUseRuleIndex))
	{
		// 핸드 마나 비용을 UI에 표시
		ManaCost->SetText(UKismetTextLibrary::Conv_IntToText(HandManaCost));

		// 덱 상태의 마나 비용을 가져옴
		int32 DeckManaCost, DeckUseRuleIndex;
		if (CardActor->GetStatusCostValueFromUseRules(ECardDataType::Deck, UStatus_Mana::StaticClass(), DeckManaCost, DeckUseRuleIndex))
		{
			// 마나 비용 차이에 따른 색상 변경
			int32 CostChangeDirection = FMath::Clamp(DeckManaCost - HandManaCost, -1, 1);
			SetManaCostColor(CostChangeDirection);
		}
	}
	else
	{
		// 마나 정보가 없으면 크리스탈 UI를 숨김
		ManaCrystalOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUW_CardVisual::SetCardFrameTint(const FSlateColor& TintColor)
{
	TypeBanner->SetBrushTintColor(TintColor);
	ManaCrystalFrame->SetBrushTintColor(TintColor);
	FrameImage->SetBrushTintColor(TintColor);
	FrameImage->SetBrushFromTexture(CardActor->GetCardFrame(ECardDataType::Hand));
}

void UUW_CardVisual::UpdateRarityGem()
{
	if (UTexture2D* RarityTexture = RarityGems.FindRef(CardActor->Rarity))
	{
		RarityGem->SetBrushFromTexture(RarityTexture);
	}
	else
	{
		RarityGem->SetBrushFromTexture(DefaultRarityGemTexture);
		GemShadow->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUW_CardVisual::SetManaCostColor(int32 CostChangeDirection)
{
	FSlateColor ManaCostColor;

	// 비용 변화에 따른 색상 설정
	switch (CostChangeDirection)
	{
	case -1: // 마나 비용이 감소한 경우
		ManaCostColor = FSlateColor(FLinearColor(1, 0, 0, 1)); // 빨강색
		break;
	case 0: // 마나 비용이 동일한 경우
		ManaCostColor = FSlateColor(FLinearColor(1, 1, 1, 1)); // 흰색
		break;
	case 1: // 마나 비용이 증가한 경우
		ManaCostColor = FSlateColor(FLinearColor(0, 1, 0, 1)); // 초록색
		break;

	default: break;;
	}

	// 설정한 색상을 UI에 적용
	ManaCost->SetColorAndOpacity(ManaCostColor);
}

void UUW_CardVisual::ToggleCardGlow_Implementation(bool bEnable, FLinearColor Color)
{
	if (bEnable)
	{
		GlowBorder->SetBrushColor(Color);
		PlayAnimation(GlowPulse, 0, 0, EUMGSequencePlayMode::Forward, GlowPulseSpeed);
	}
	else
	{
		StopAnimation(GlowPulse);
		PlayAnimation(HideGlow);
	}
}

void UUW_CardVisual::AnimateCardAppear_Implementation(EUMGSequencePlayMode::Type PlayMode)
{
	PlayAnimation(Appear, 0.0, 1, PlayMode);
}

void UUW_CardVisual::AnimateCardNotify_Implementation(EUMGSequencePlayMode::Type PlayMode)
{
	PlayAnimation(Notify, PlayMode);
}

void UUW_CardVisual::AnimateCardExhaust_Implementation(EUMGSequencePlayMode::Type PlayMode)
{
	PlayAnimation(ExhaustUp, 0, 1, PlayMode, 1.0f);
}

void UUW_CardVisual::UpdateCardWidget_Implementation(ACardBase* InCardActor)
{
	// 카드 액터를 업데이트
	CardActor = InCardActor;

	// 마나 크리스탈 정보를 갱신
	UpdateManaCrystal();

	// 카드 프레임 틴트를 가져와서 필요한 UI 요소에 적용
	const FSlateColor& CardFrameTint = CardActor->GetCardFrameTint(ECardDataType::Hand);
	SetCardFrameTint(CardFrameTint);

	// 카드 이미지와 이름을 설정
	CardImage->SetBrushFromTexture(CardActor->GetCardPortrait(ECardDataType::Hand));
	CardName->SetText(CardActor->GetCardName(ECardDataType::Hand));

	// 카드 효과를 텍스트로 대체
	ReplacePowerValues();

	// 카드 타입을 UI에 반영
	if (TypeNames.Contains(CardActor->CardType))
	{
		TypeText->SetText(TypeNames[CardActor->CardType]);
	}

	// 카드의 희귀성 보석을 UI에 반영
	UpdateRarityGem();
}

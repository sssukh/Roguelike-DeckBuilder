
#include "UI/UW_CardVisual.h"

#include "CardSystem/CardBase.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Libraries/AssetPath.h"
#include "StatusSystem/Status_Mana.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UUW_CardVisual::UUW_CardVisual(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), ManaCrystalOverlay(nullptr), GlowBorder(nullptr),
                                                                              TypeBanner(nullptr), RarityGem(nullptr), ManaCrystalFrame(nullptr), GemShadow(nullptr), FrameImage(nullptr),
                                                                              CardImage(nullptr), CardName(nullptr), ManaCost(nullptr), TypeText(nullptr), RichDescription(nullptr),
                                                                              DefaultRarityGemTexture(nullptr),
                                                                              CardActor(nullptr)
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
		Execute_UpdateCardWidget(this, CardActor);
	}
}

void UUW_CardVisual::ReplacePowerValues()
{
	FString LocalDescriptionString = CardActor->GetCardDescription(ECardDataType::Hand).ToString();

	const TArray<FCardEffect>& CardEffects = CardActor->GetCardEffects(ECardDataType::Hand);

	int32 IndexI = 0;
	for (const FCardEffect& CardEffect : CardEffects)
	{
		FString From = UKismetStringLibrary::Concat_StrStr(FString(TEXT("<b>")), UKismetStringLibrary::Concat_StrStr(FString::FromInt(CardEffect.EffectValue), FString(TEXT("</>"))));
		FString To = UKismetStringLibrary::Concat_StrStr(FString(TEXT("*")), FString::FromInt(IndexI));
		LocalDescriptionString = LocalDescriptionString.Replace(*From, *To, ESearchCase::IgnoreCase);
		IndexI++;
	}

	const TArray<FStatusData>& StartingStatuses = CardActor->GetCardStartingStatuses(ECardDataType::Hand);
	if (StartingStatuses.Num() > 0)
	{
		int32 IndexJ = 0;
		for (const FStatusData& StartingStatus : StartingStatuses)
		{
			FString From = UKismetStringLibrary::Concat_StrStr(FString(TEXT("#")), FString::FromInt(IndexJ));
			FString To = UKismetStringLibrary::Concat_StrStr(FString(TEXT("<b>")), UKismetStringLibrary::Concat_StrStr(FString::FromInt(StartingStatus.Value), FString(TEXT("</>"))));
			LocalDescriptionString = LocalDescriptionString.Replace(*From, *To, ESearchCase::IgnoreCase);
			IndexJ++;
		}
	}


	// 대체할 문자열과 대체될 문자열을 정의합니다.
	FString FromString = "<br>"; // 소스에서 대체할 부분
	FString ToString = "\r\n"; // 대체할 문자열

	// Replace 함수 호출을 통해 문자열에서 대체 작업을 수행합니다.
	LocalDescriptionString = LocalDescriptionString.Replace(*FromString, *ToString, ESearchCase::IgnoreCase);
	RichDescription->SetText(FText::FromString(LocalDescriptionString));
}

void UUW_CardVisual::UpdateManaCrystal()
{
	int32 HandUseRuleIndex;
	int32 HandManaCst;
	if (CardActor->GetStatusCostValueFromUseRules(ECardDataType::Hand, UStatus_Mana::StaticClass(), HandManaCst, HandUseRuleIndex))
	{
		ManaCost->SetText(UKismetTextLibrary::Conv_IntToText(HandManaCst));

		int32 DeckUseRuleIndex;
		int32 DeckManaCst;
		if (CardActor->GetStatusCostValueFromUseRules(ECardDataType::Deck, UStatus_Mana::StaticClass(), DeckManaCst, DeckUseRuleIndex))
		{
			int32 CostConsumption = (FMath::Sign<int32>(DeckManaCst - HandManaCst) + 1);

			FSlateColor ManaCostColor;
			if (CostConsumption == 0) ManaCostColor = FSlateColor(FLinearColor(1, 0, 0, 1));
			else if (CostConsumption == 1) ManaCostColor = FSlateColor(FLinearColor(1, 1, 1, 1));
			else if (CostConsumption == 2) ManaCostColor = FSlateColor(FLinearColor(0, 1, 0, 1));

			ManaCost->SetColorAndOpacity(ManaCostColor);
		}
	}
	else
	{
		ManaCrystalOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
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
	CardActor = InCardActor;
	UpdateManaCrystal();

	const FSlateColor& CardFrameTint = CardActor->GetCardFrameTint(ECardDataType::Hand);

	TypeBanner->SetBrushTintColor(CardFrameTint);

	ManaCrystalFrame->SetBrushTintColor(CardFrameTint);

	FrameImage->SetBrushTintColor(CardFrameTint);
	FrameImage->SetBrushFromTexture(CardActor->GetCardFrame(ECardDataType::Hand));

	CardImage->SetBrushFromTexture(CardActor->GetCardPortrait(ECardDataType::Hand));
	CardName->SetText(CardActor->GetCardName(ECardDataType::Hand));

	ReplacePowerValues();

	if (TypeNames.Contains(CardActor->CardType))
	{
		TypeText->SetText(TypeNames[CardActor->CardType]);
	}

	if (RarityGems.Contains(CardActor->Rarity))
	{
		RarityGem->SetBrushFromTexture(RarityGems[CardActor->Rarity]);
	}
	else
	{
		RarityGem->SetBrushFromTexture(DefaultRarityGemTexture);
		GemShadow->SetVisibility(ESlateVisibility::Hidden);
	}
}

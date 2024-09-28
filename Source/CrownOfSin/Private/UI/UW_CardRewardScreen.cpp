#include "UI/UW_CardRewardScreen.h"

#include "CardSystem/CardBase.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_CardListCard.h"
#include "Utilities/CosLog.h"

UUW_CardRewardScreen::UUW_CardRewardScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), Image_81(nullptr), RewardBox(nullptr), SkipButton(nullptr), SkipOverlay(nullptr),
                                                                                          Title(nullptr),
                                                                                          FadeIn(nullptr),
                                                                                          LoopIndex(0)
{
	static ConstructorHelpers::FClassFinder<UUW_CardListCard> WBP_CardListCardClass_C(*AssetPath::Blueprint::WBP_CardListCard_C);
	if (WBP_CardListCardClass_C.Succeeded())
	{
		WBP_CardListCardClass = WBP_CardListCardClass_C.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_CardListCardClass를 찾지 못했습니다"));
	}
}

void UUW_CardRewardScreen::UpdateRewardScreen(const TArray<ACardBase*>& Cards, const FText& InTitle, bool bAllowSkip)
{
	SkipOverlay->SetVisibility(bAllowSkip ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	RewardBox->ClearChildren();

	Title->SetText(InTitle);

	SetVisibility(ESlateVisibility::Visible);

	for (int i = 0; i < Cards.Num(); i++)
	{
		if (i < RewardOptionCount)
		{
			UUW_CardListCard* NewCardListCardWidget = Cast<UUW_CardListCard>(CreateWidget(GetWorld(), WBP_CardListCardClass));
			NewCardListCardWidget->CardActor = Cards[i];
			RewardBox->AddChild(NewCardListCardWidget);
			NewCardListCardWidget->OnCardClicked.AddDynamic(this, &UUW_CardRewardScreen::ReturnReward);
		}
	}

	ShowCardOptions();
}

void UUW_CardRewardScreen::ReturnReward(UUW_CardListCard* CardListCard, ACardBase* CardActor)
{
	for (UWidget* Widget : RewardBox->GetAllChildren())
	{
		UUW_CardListCard* CastCardListCard = Cast<UUW_CardListCard>(Widget);
		if (!CastCardListCard) continue;

		CastCardListCard->SetVisibility(ESlateVisibility::HitTestInvisible);
		CastCardListCard->bBlockHoverAnim = true;
		CastCardListCard->PlayAnimation(CardListCard != CastCardListCard ? CardListCard->FadeOut : CardListCard->Select);
	}

	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&, CardActor]()
	{
		PlayAnimationReverse(FadeIn);

		FTimerHandle DelayHandle2;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle2, FTimerDelegate::CreateLambda([&, CardActor]()
		{
			SetVisibility(ESlateVisibility::Collapsed);
			if (OnReturnSelectedCardInRewardScreen.IsBound())
				OnReturnSelectedCardInRewardScreen.Broadcast(false, CardActor);

			GetWorld()->GetTimerManager().ClearTimer(DelayHandle2);
		}), 0.1f, false);

		GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
	}), 0.4f, false);
}
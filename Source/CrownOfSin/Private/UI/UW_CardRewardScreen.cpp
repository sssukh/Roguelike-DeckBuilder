// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_CardRewardScreen.h"

#include "CardSystem/CardBase.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "UI/UW_CardListCard.h"

UUW_CardRewardScreen::UUW_CardRewardScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_CardRewardScreen::UpdateRewardScreen(TArray<ACardBase*>& Cards, const FText& InTitle, bool bAllowSkip)
{
	if(bAllowSkip)
	{
		SkipOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SkipOverlay->SetVisibility(ESlateVisibility::Hidden);
	}

	RewardBox->ClearChildren();

	Title->SetText(InTitle);

	SetVisibility(ESlateVisibility::Visible);

	for (int idx=0; idx < Cards.Num(); idx++)
	{
		if(idx<RewardOptionCount)
		{
			UUW_CardListCard* NewCardListCardWidget = Cast<UUW_CardListCard>(CreateWidget(GetWorld(),UUW_CardListCard::StaticClass()));

			if(NewCardListCardWidget)
			{
				NewCardListCardWidget->CardActor = Cards[idx];

				RewardBox->AddChild(NewCardListCardWidget);

				NewCardListCardWidget->OnCardClicked.AddDynamic(this,&UUW_CardRewardScreen::ReturnReward);
			}
		}
	}
}

void UUW_CardRewardScreen::ReturnReward(UUW_CardListCard* CardListCard, ACardBase* CardActor)
{
}

﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_CardRewardScreen.h"

#include "CardSystem/CardBase.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_CardListCard.h"
#include "Utilities/CosLog.h"

UUW_CardRewardScreen::UUW_CardRewardScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUW_CardListCard> WBP_CardListCard(*AssetPath::Blueprint::WBP_CardListCard);
	if(WBP_CardListCard.Succeeded())
	{
		CardListCardClass = WBP_CardListCard.Class;
	}
	else
	{
		COS_SCREEN(TEXT("WBP_CardListCard를 로드하는데 실패했습니다."));
	}
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
			UUW_CardListCard* NewCardListCardWidget = Cast<UUW_CardListCard>(CreateWidget(GetWorld(),CardListCardClass));

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
	for (UWidget* Widget : RewardBox->GetAllChildren())
	{
		UUW_CardListCard* CardListCardCasted = Cast<UUW_CardListCard>(Widget);

		CardListCardCasted->SetVisibility(ESlateVisibility::HitTestInvisible);

		CardListCardCasted->bBlockHoverAnim = true;

		if(CardListCard!=CardListCardCasted)
		{
			CardListCard->PlayAnimation(CardListCard->Select);
		}
		else
		{
			CardListCard->PlayAnimation(CardListCard->FadeOut);
		}
	}

	FTimerHandle DelayHandle;
	// Delay 0.4f

	GetWorld()->GetTimerManager().SetTimer(DelayHandle,FTimerDelegate::CreateLambda([&]()
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
	}), 4.0f,false);
	

	PlayAnimationReverse(FadeIn);

	// Delay 0.1f
	GetWorld()->GetTimerManager().SetTimer(DelayHandle,FTimerDelegate::CreateLambda([&]()
		{
			GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
		}), 1.0f,false);
	
	
	

	SetVisibility(ESlateVisibility::Collapsed);

	OnReturnSelectedCardInRewardScreen.Broadcast(false,CardActor);
}

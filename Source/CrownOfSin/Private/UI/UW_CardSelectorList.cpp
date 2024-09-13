﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_CardSelectorList.h"

#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "UI/UW_CardListCard.h"
#include "Utilities/CosLog.h"

UUW_CardSelectorList::UUW_CardSelectorList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_CardSelectorList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(!bDebugMode)
		return;

	for(int i=0;i<20;++i)
	{
		UUserWidget* CardListCard = CreateWidget(GetWorld(),CardListClass);

		CardPanel->AddChildToUniformGrid(CardListCard,i/RowLength,i%RowLength);
	}
}



void UUW_CardSelectorList::UpdateCardList(TArray<ACardBase*>& Cards, const FText& Title)
{
	PileName->SetText(Title);

	CardPanel->ClearChildren();

	//수정해야합니다.
	COS_LOG_SCREEN_ERROR(TEXT(" UUW_CardSelectorList::UpdateCardList ()  함수를 구현해야합니다."));
	// for (int index=0; index<Cards.Num(); index++)
	// {
	// 	UUW_CardListCard* CardList = Cast<UUW_CardListCard>(this);
	// 	CardList->CardActor = Cards[index];
	//
	// 	CardPanel->AddChildToUniformGrid(CardList,index/RowLength,index%RowLength);
	//
	// 	CardList->OnCardClicked.AddDynamic(this,&UUW_CardSelectorList::ReturnCardWhenClicked);
	//
	// 	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	// }
}

void UUW_CardSelectorList::ReturnCardWhenClicked(UUW_CardListCard* CardListCard, ACardBase* CardActor)
{
	OnReturnSelectedCardInSelectorList.Broadcast(CardActor);

	SetVisibility(ESlateVisibility::Collapsed);
}



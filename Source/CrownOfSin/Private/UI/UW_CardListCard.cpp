﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_CardListCard.h"
#include "CardSystem/CardBase.h"
#include "Components/SizeBox.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "UI/UW_CardVisual.h"
#include "UI/UW_ToolTipList.h"

UUW_CardListCard::UUW_CardListCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_CardListCard::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetCardVisualWidget(CardActor);
}

void UUW_CardListCard::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(this->GetClass()->ImplementsInterface(UInterface_Utility::StaticClass()))
	{
		IInterface_Utility::Execute_Initialize(this,FGameplayTagContainer());
	}
}

UUserWidget* UUW_CardListCard::SetCardVisualWidget(ACardBase* InCardActor)
{
	CardActor = InCardActor;

	// CardVisual 이미 있으면 제거
	if(IsValid(CardVisual))
	{
		CardVisual->RemoveFromParent();
	}

	// CardActor 유효하면 CardVisualClass 지정
	if(IsValid(CardActor))
	{
		CardVisualClass = CardActor->GetCardVisualWidget(ECardDataType::Deck);
	}

	// CardVisual 새로 생성 및 설정
	CardVisual = CreateWidget(GetWorld(),CardVisualClass);

	CardVisualBox->AddChild(CardVisual);

	CardVisual->SetVisibility(ESlateVisibility::HitTestInvisible);

	return CardVisual;
}

void UUW_CardListCard::Initialize_Implementation(const FGameplayTagContainer& Tags)
{
	if(!IsValid(CardActor))
		return;
	
	if(CardVisual->GetClass()->ImplementsInterface(UInterface_CardWidget::StaticClass()))
	{
		// TODO : 구현해야한다.
		IInterface_CardWidget::Execute_UpdateCardWidget(CardVisual,CardActor);
	}

	for (FDataTableRowHandle Tooltip : CardActor->GetCardTooltips(ECardDataType::Hand))
	{
		WBP_TooltipList->AddToolTipFromData(Tooltip,0);
	}
}

void UUW_CardListCard::UpdateCardWidget_Implementation(ACardBase* InCardActor)
{
	UUW_CardVisual* CardVisualCasted = Cast<UUW_CardVisual>(SetCardVisualWidget(InCardActor));

	CardVisualCasted->UpdateCardWidget_Implementation(CardActor);
}

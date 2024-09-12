// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_CardListCard.h"
#include "CardSystem/CardBase.h"
#include "Components/SizeBox.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
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

	IInterface_Utility::Execute_Initialize(this,FGameplayTagContainer());
}

UUserWidget* UUW_CardListCard::SetCardVisualWidget(ACardBase* InCardActor)
{
	CardActor = InCardActor;

	if(IsValid(CardVisual))
	{
		CardVisual->RemoveFromParent();
	}

	if(IsValid(CardActor))
	{
		CardVisualClass = CardVisualClass = CardActor->GetCardVisualWidget(ECardDataType::Deck);
	}
	
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
	SetCardVisualWidget(InCardActor);
}

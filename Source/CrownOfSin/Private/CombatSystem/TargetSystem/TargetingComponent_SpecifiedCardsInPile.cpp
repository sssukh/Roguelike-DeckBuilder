// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_SpecifiedCardsInPile.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_CardHand.h"
#include "UI/UW_CardSelectorList.h"
#include "UI/UW_Layout_Cos.h"
#include "CardSystem/CardBase.h"


// Sets default values for this component's properties
UTargetingComponent_SpecifiedCardsInPile::UTargetingComponent_SpecifiedCardsInPile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UTargetingComponent_SpecifiedCardsInPile::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
	const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets)
{
	if(bPreview)
		return false;

	if(!CardEffect.TargetComponent->IsChildOf(UPileComponent::StaticClass()))
	{
		return false;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UPileComponent* PileComponent = Cast<UPileComponent>(CardPlayer->GetComponentByClass(UPileComponent::StaticClass()));

	if(PileComponent->Cards.IsEmpty())
	{
		return false;
	}

	CardPlayer->PlayerUI->WBP_Hand->bSelectionMode = true;

	BindToCardConfirm(CardPlayer->PlayerUI->WBP_CardSelectorList);

	FText Title = FText::FromString(FString(TEXT("Choose a Card"))); 

	// TODO 구현해야됨
	CardPlayer->PlayerUI->WBP_CardSelectorList->UpdateCardList(PileComponent->Cards,Title);

	return true;
}

void UTargetingComponent_SpecifiedCardsInPile::BindToCardConfirm(UUW_CardSelectorList* CardList)
{
	CardSelectorList = CardList;
	CardList->OnReturnSelectedCardInSelectorList.AddDynamic(this, &UTargetingComponent_SpecifiedCardsInPile::ValidateAndTransferSelectedCard);
}

void UTargetingComponent_SpecifiedCardsInPile::ValidateAndTransferSelectedCard(ACardBase* Card)
{
	CardSelectorList->OnReturnSelectedCardInSelectorList.RemoveDynamic(this,&UTargetingComponent_SpecifiedCardsInPile::ValidateAndTransferSelectedCard);

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	CardPlayer->PlayerUI->WBP_Hand->bSelectionMode = false;

	TArray<AActor*> Cards;
	Cards.Add(Card);
	
	OnInputTargetsReceived.Broadcast(Cards);
}





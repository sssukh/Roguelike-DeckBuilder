// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_SpecifiedCardsInHand.h"

#include "UI/UW_CardHand.h"
#include "Utilities/CosLog.h"
#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Core/GameplayTagComponent.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "UI/UW_Layout_Cos.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UTargetingComponent_SpecifiedCardsInHand::UTargetingComponent_SpecifiedCardsInHand()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UTargetingComponent_SpecifiedCardsInHand::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                                const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{

	OutValidTargets.Reset();
	
	if(bPreview)
	{
		return true;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	TArray<ACardBase*> ValidCards;
	
	UPileComponent* PileComponent = Cast<UPileComponent>(CardPlayer->GetComponentByClass(UPileComponent::StaticClass()));

	for (ACardBase* CardInPile : PileComponent->Cards)
	{
		UGameplayTagComponent* GameplayTagComponent = Cast<UGameplayTagComponent>(CardInPile->GetComponentByClass(UGameplayTagComponent::StaticClass()));

		bool bCardEffectTagsHasAnyTagsinCardInPile = CardEffect.GameplayTags.HasAnyExact(GameplayTagComponent->GameplayTags);
		
		if(GameplayTagComponent && (bCardEffectTagsHasAnyTagsinCardInPile || CardEffect.GameplayTags == EmptyTagContainer))
		{
			ValidCards.Add(CardInPile);
		}
	}

	if(OutValidTargets.IsEmpty())
	{
		return false;
	}

	if (OutValidTargets.Num()>CardEffect.EffectValue)
	{
		UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_CardSelectionMode,Card);

		UUW_CardHand* CardhandWidget = CardPlayer->PlayerUI->WBP_CardHand;

		BindToCardConfirm(CardhandWidget);

		CardhandWidget->EnableSelectionMode(CardEffect.EffectValue,CardEffect.GameplayTags);

		OutValidTargets.Reset();

		return true;
	}
	else
	{
		ValidateAndTransferSelectedCards(ValidCards);

		for (ACardBase* ValidCard : ValidCards)
		{
			OutValidTargets.Add(ValidCard);
		}
		
		return true;
	}
}

void UTargetingComponent_SpecifiedCardsInHand::BindToCardConfirm(UUW_CardHand* Hand)
{
	if(!Hand)
	{
		COS_SCREEN(TEXT("전달받은 위젯 CardHand가 유효하지 않습니다. in %s"),*this->GetName());
		return;
	}
	
	BoundHand = Hand;

	BoundHand->OnReturnSelectedCards.AddDynamic(this,&UTargetingComponent_SpecifiedCardsInHand::ValidateAndTransferSelectedCards);
}

void UTargetingComponent_SpecifiedCardsInHand::ValidateAndTransferSelectedCards(const TArray<ACardBase*>& Cards)
{
	if(BoundHand)
	{
		BoundHand->OnReturnSelectedCards.RemoveDynamic(this,&UTargetingComponent_SpecifiedCardsInHand::ValidateAndTransferSelectedCards);
	}
	TArray<AActor*> SelectedCards;
	for (ACardBase* Card : Cards)
	{
		SelectedCards.Add(Card);
	}
	OnInputTargetsReceived.Broadcast(SelectedCards);
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_AllValidCardsInHand.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Core/GameplayTagComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UTargetingComponent_AllValidCardsInHand::UTargetingComponent_AllValidCardsInHand()
{
	
}

bool UTargetingComponent_AllValidCardsInHand::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect,
                                                               ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets)
{
	ValidTargets.Reset();
	
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UPileComponent* PileComp = Cast<UPileComponent>(CardPlayer->GetComponentByClass(UPileComponent::StaticClass()));

	for (ACardBase* CardInPile : PileComp->Cards)
	{
		UGameplayTagComponent* CardTags = Cast<UGameplayTagComponent>(CardInPile->GetComponentByClass(UGameplayTagComponent::StaticClass()));

		if(CardEffect.GameplayTags.HasAnyExact(CardTags->GameplayTags))
		{
			ValidTargets.Add(CardInPile);
		}
	}
	
	return false;
}





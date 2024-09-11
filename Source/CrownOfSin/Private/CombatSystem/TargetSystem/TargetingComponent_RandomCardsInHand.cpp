// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_RandomCardsInHand.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "Core/GameplayTagComponent.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UTargetingComponent_RandomCardsInHand::UTargetingComponent_RandomCardsInHand()
{
	
}

bool UTargetingComponent_RandomCardsInHand::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
	const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets)
{
	ValidTargets.Reset();

	TArray<AActor*> FoundTargets;
	
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UPileComponent* PileComp = Cast<UPileComponent>(CardPlayer->GetComponentByClass(UPileComponent::StaticClass()));

	for (ACardBase* CardInPile : PileComp->Cards)
	{
		UGameplayTagComponent* GameplayTagComponent = Cast<UGameplayTagComponent>(CardInPile->GetComponentByClass(UGameplayTagComponent::StaticClass()));
		bool bCardHasGameTag = CardEffect.GameplayTags.HasAny(GameplayTagComponent->GameplayTags);

		if(bCardHasGameTag || CardEffect.GameplayTags.Num()==0)
		{
			FoundTargets.Add(CardInPile);
		}
	}

	while (FoundTargets.Num()>0 && ValidTargets.Num()<CardEffect.EffectValue)
	{
		int32 Index;
		
		UFunctionLibrary_ArrayUtils::GetRandomElementFromArray(FoundTargets,Index);

		ValidTargets.Add(FoundTargets[Index]);
		
		FoundTargets.RemoveAt(Index);
	}

	return ValidTargets.Num()>0;
}




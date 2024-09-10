// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_CardOwner.h"

#include "CardSystem/CardBase.h"


// Sets default values for this component's properties
UTargetingComponent_CardOwner::UTargetingComponent_CardOwner()
{
	
}

bool UTargetingComponent_CardOwner::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect,
                                                     ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets)
{
	ValidTargets.Reset();
	
	if(!Card->GetOwner())
		return false;

	ValidTargets.Add(Card->GetOwner());
	return true;
}




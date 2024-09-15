// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_AllValidTargets.h"


// Sets default values for this component's properties
UTargetingComponent_AllValidTargets::UTargetingComponent_AllValidTargets()
{
	
}

bool UTargetingComponent_AllValidTargets::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                           const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	OutValidTargets = FindAllMinionsWithMatchingTags(CardEffect.GameplayTags);
	
	return false;
}





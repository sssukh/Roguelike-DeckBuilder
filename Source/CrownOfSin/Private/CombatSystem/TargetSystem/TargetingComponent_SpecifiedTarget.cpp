// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem//TargetingComponent_SpecifiedTarget.h"


// Sets default values for this component's properties
UTargetingComponent_SpecifiedTarget::UTargetingComponent_SpecifiedTarget()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UTargetingComponent_SpecifiedTarget::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                           const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	OutValidTargets.Reset();
	
	for (AActor* Target : SpecifiedTargets)
	{
		if(IsValid(Target) && CheckTargetHasGameplayTag(Target,CardEffect))
		{
			OutValidTargets.Add(Target);
		}
	}

	return false;
}





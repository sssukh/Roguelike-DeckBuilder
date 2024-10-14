// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_SameAsPrevious.h"

#include "CardSystem/CardBase.h"


// Sets default values for this component's properties
UTargetingComponent_SameAsPrevious::UTargetingComponent_SameAsPrevious()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UTargetingComponent_SameAsPrevious::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                          const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	TArray<AActor*> FoundTargets;
	
	for (AActor* Target : Card->CurrentValidTargets)
	{
		if(IsValid(Target))
		{
			FoundTargets.Add(Target);
		}
	}
	
	OutValidTargets = FoundTargets;
	
	return FoundTargets.Num() > 0;
}




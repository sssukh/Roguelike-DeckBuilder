// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_Self.h"
#include "CardSystem/CardBase.h"


// Sets default values for this component's properties
UTargetingComponent_Self::UTargetingComponent_Self()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UTargetingComponent_Self::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect,
                                                AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	OutValidTargets.Reset();

	OutValidTargets.Add(Card);

	return true;
}





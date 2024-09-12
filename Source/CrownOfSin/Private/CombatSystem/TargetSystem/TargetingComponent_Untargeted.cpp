// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_Untargeted.h"


// Sets default values for this component's properties
UTargetingComponent_Untargeted::UTargetingComponent_Untargeted()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UTargetingComponent_Untargeted::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect,
                                                      ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets)
{
	TArray<AActor*> NewTargets;
	NewTargets.Add(nullptr);
	ValidTargets = NewTargets;
	return true;
}

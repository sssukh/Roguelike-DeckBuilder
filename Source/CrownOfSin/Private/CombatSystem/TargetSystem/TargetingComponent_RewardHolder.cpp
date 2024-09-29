// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_RewardHolder.h"

#include "Core/RewardHolder.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTargetingComponent_RewardHolder::UTargetingComponent_RewardHolder()
{
}

bool UTargetingComponent_RewardHolder::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	OutValidTargets.Empty();
	
	AActor* RewardHolder = UGameplayStatics::GetActorOfClass(this, ARewardHolder::StaticClass());
	OutValidTargets.Add(RewardHolder);
	return true;
}
